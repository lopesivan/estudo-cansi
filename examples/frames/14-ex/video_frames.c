#include <ft2build.h>
#include FT_FREETYPE_H
FT_Library ft_library;
FT_Face ft_face;

#include FT_GLYPH_H  // Se quiser trabalhar com glifos manualmente
#include FT_BITMAP_H // Se quiser acesso ao bitmap diretamente
/* --------------------------- ------------------------ ------------------- */

#include <libavcodec/avcodec.h> /* Biblioteca para lidar com codecs de vídeo/áudio */
#include <libavformat/avformat.h> /* Biblioteca para manipular formatos de arquivo multimídia */
#include <libavutil/imgutils.h> /* Funções auxiliares para imagens */
#include <libswscale/swscale.h> /* Biblioteca para converter formatos de pixel e redimensionamento */

#include "frames.h" /* Biblioteca personalizada para gerenciar IDs de frame */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_FRAMES 5

/* Estrutura que representa uma imagem RGB mantida em memória */
typedef struct
{
    int id;         /* Identificador lógico do frame */
    int largura;    /* Largura da imagem em pixels */
    int altura;     /* Altura da imagem em pixels */
    uint8_t *dados; /* Ponteiro para os dados RGB (tamanho: largura * altura *
                       3) */
} ImagemRGB;

/* Inicializa o pool de frame_ids usando uma lista encadeada (list.c).
 * Isso simula uma "fila de recursos" disponíveis para controlar alocação.
 */
void init_frame_pool(List *pool, int total)
{
    /* Inicializa a lista e define 'free' como função
                              de liberação dos dados */
    list_init(pool, free);
    for (int i = total - 1; i >= 0; i--)
    {
        int *f = malloc(sizeof(int)); /* Aloca um inteiro na heap */
        *f = i;
        /* Insere no início da lista (ordem reversa) */
        list_ins_next(pool, NULL, f);
    }
}

/* Salva uma imagem RGB em formato PPM (imagem bruta sem compressão) no disco
 */
void salvar_imagem_em_arquivo(const ImagemRGB *img)
{
    char nome[64];
    snprintf(nome, sizeof(nome), "imagem_%03d.ppm",
             img->id); /* Gera nome do arquivo */

    FILE *f = fopen(nome, "wb");
    fprintf(f, "P6\n%d %d\n255\n", img->largura,
            img->altura); /* Cabeçalho do formato PPM */

    /* Grava linha por linha os dados RGB */
    for (int y = 0; y < img->altura; y++)
    {
        fwrite(img->dados + y * img->largura * 3, 1, img->largura * 3, f);
    }
    fclose(f);
}

void desenhar_faixa(ImagemRGB *img, int y_inicial, int altura, int r, int g,
                    int b)
{
    int y_fim = y_inicial + altura;
    if (y_inicial < 0)
        y_inicial = 0;
    if (y_fim > img->altura)
        y_fim = img->altura;

    for (int y = y_inicial; y < y_fim; y++)
    {
        for (int x = 0; x < img->largura; x++)
        {
            int offset = (y * img->largura + x) * 3;
            img->dados[offset + 0] = r;
            img->dados[offset + 1] = g;
            img->dados[offset + 2] = b;
        }
    }
}

/* Inverte as cores de toda a imagem (efeito negativo): */
void inverter_cores(ImagemRGB *img)
{
    int total_pixels = img->largura * img->altura;
    for (int i = 0; i < total_pixels * 3; i++)
    {
        img->dados[i] = 255 - img->dados[i];
    }
}

/* reduz o brilho da imagem */
void escurecer_imagem(ImagemRGB *img, float fator)
{
    if (fator < 0.0f)
        fator = 0.0f;
    if (fator > 1.0f)
        fator = 1.0f;

    int total_pixels = img->largura * img->altura;
    for (int i = 0; i < total_pixels * 3; i++)
    {
        img->dados[i] = (uint8_t)(img->dados[i] * fator);
    }
}

void desenhar_texto(ImagemRGB *img, int x, int y, const char *texto,
                    uint8_t r, uint8_t g, uint8_t b)
{
    FT_GlyphSlot slot = ft_face->glyph;
    int pen_x = x;
    int pen_y = y;

    for (const char *p = texto; *p; p++)
    {
        // Obtém o índice do glifo
        FT_UInt glyph_index = FT_Get_Char_Index(ft_face, *p);

        // Carrega o glifo
        if (FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT))
            continue;

        // Renderiza o glifo em bitmap
        if (FT_Render_Glyph(ft_face->glyph, FT_RENDER_MODE_NORMAL))
            continue;

        FT_Bitmap *bitmap = &slot->bitmap;

        // Posição do glifo na imagem
        int x_offset = pen_x + slot->bitmap_left;
        int y_offset = pen_y - slot->bitmap_top;

        for (int row = 0; row < bitmap->rows; row++)
        {
            for (int col = 0; col < bitmap->width; col++)
            {
                int img_x = x_offset + col;
                int img_y = y_offset + row;

                if (img_x < 0 || img_x >= img->largura || img_y < 0 ||
                    img_y >= img->altura)
                    continue;

                uint8_t alpha = bitmap->buffer[row * bitmap->pitch + col];
                float alpha_f = alpha / 255.0f;

                int offset = (img_y * img->largura + img_x) * 3;
                img->dados[offset + 0] =
                    (uint8_t)(r * alpha_f +
                              img->dados[offset + 0] * (1 - alpha_f));
                img->dados[offset + 1] =
                    (uint8_t)(g * alpha_f +
                              img->dados[offset + 1] * (1 - alpha_f));
                img->dados[offset + 2] =
                    (uint8_t)(b * alpha_f +
                              img->dados[offset + 2] * (1 - alpha_f));
            }
        }

        // Avança a posição do "pen"
        pen_x += slot->advance.x >> 6;
    }
}

/* Função principal do programa */
int main(int argc, char *argv[])
{
    /* Verifica se o caminho do vídeo foi passado como argumento */
    if (argc < 2)
    {
        printf("Uso: %s arquivo.mp4\n", argv[0]);
        return -1;
    }

    AVFormatContext *pFormatCtx = NULL; /* Contexto do arquivo de mídia */
    AVCodecContext *pCodecCtx = NULL;   /* Contexto do codec de vídeo */
    AVFrame *pFrame = NULL,
            *pFrameRGB = NULL; /* Frames original e convertido para RGB */
    AVPacket packet;           /* Pacote de dados lido do vídeo */
    struct SwsContext *sws_ctx = NULL; /* Contexto de conversão de cor */

    /* Abre o arquivo de vídeo */
    avformat_open_input(&pFormatCtx, argv[1], NULL, NULL);
    avformat_find_stream_info(pFormatCtx,
                              NULL); /* Lê as informações do arquivo */

    int videoStream = -1;
    /* Procura o índice do stream de vídeo no arquivo */
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codecpar->codec_type ==
            AVMEDIA_TYPE_VIDEO)
            videoStream = i;

    if (videoStream == -1)
        return -1; /* Se nenhum stream de vídeo for encontrado, encerra o
                      programa */

    /* Encontra o decodificador apropriado */
    const AVCodec *pCodec = avcodec_find_decoder(
        pFormatCtx->streams[videoStream]->codecpar->codec_id);

    /* Inicializa o contexto do codec e copia os parâmetros do stream */
    pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx,
                                  pFormatCtx->streams[videoStream]->codecpar);
    avcodec_open2(pCodecCtx, pCodec, NULL); /* Abre o codec */

    pFrame = av_frame_alloc();    /* Aloca estrutura para frame original */
    pFrameRGB = av_frame_alloc(); /* Aloca estrutura para frame convertido */

    int largura = pCodecCtx->width;
    int altura = pCodecCtx->height;
    int numBytes =
        av_image_get_buffer_size(AV_PIX_FMT_RGB24, largura, altura, 1);
    uint8_t *buffer =
        (uint8_t *)av_malloc(numBytes); /* Aloca buffer para imagem RGB */

    /* Associa o buffer à estrutura pFrameRGB */
    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer,
                         AV_PIX_FMT_RGB24, largura, altura, 1);

    /* Inicializa o contexto de conversão de cores (de YUV para RGB) */
    sws_ctx =
        sws_getContext(largura, altura, pCodecCtx->pix_fmt, largura, altura,
                       AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    List frame_pool;         /* Pool de frame_ids disponíveis */
    List imagens_em_memoria; /* Lista de imagens capturadas na memória */
    init_frame_pool(
        &frame_pool,
        NUMBER_OF_FRAMES); /* Cria pool com NUMBER_OF_FRAMES slots */
    list_init(
        &imagens_em_memoria,
        free); /* Inicializa lista para armazenar ponteiros de ImagemRGB */

    /* Estima o número total de frames do vídeo */
    int64_t total_frames = pFormatCtx->streams[videoStream]->nb_frames;
    if (total_frames == 0)
    {
        double duration = pFormatCtx->duration / (double)AV_TIME_BASE;
        AVRational framerate = pFormatCtx->streams[videoStream]->r_frame_rate;
        total_frames =
            (int64_t)(duration *
                      av_q2d(framerate)); /* fallback: duração * FPS */
    }

    int intervalo =
        total_frames / NUMBER_OF_FRAMES; /* Define espaçamento para capturar
                                            NUMBER_OF_FRAMES amostras */
    if (intervalo < 1)
        intervalo = 1; /* Garante intervalo mínimo válido */

    int frame_count = 0;
    /* Loop de leitura do vídeo frame a frame */
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == videoStream)
        {
            avcodec_send_packet(pCodecCtx, &packet);
            if (avcodec_receive_frame(pCodecCtx, pFrame) == 0)
            {
                /* Salva o frame somente se for múltiplo do intervalo */
                if (frame_count % intervalo == 0)
                {
                    printf("frame: %d\n", frame_count);

                    int id = alloc_frame(
                        &frame_pool); /* Aloca um frame_id disponível */
                    if (id != -1)
                    {
                        /* Converte frame original para RGB */
                        sws_scale(sws_ctx,
                                  (uint8_t const *const *)pFrame->data,
                                  pFrame->linesize, 0, altura,
                                  pFrameRGB->data, pFrameRGB->linesize);

                        /* Cria e preenche estrutura de imagem RGB */
                        ImagemRGB *img = malloc(sizeof(ImagemRGB));
                        img->id = id;
                        img->largura = largura;
                        img->altura = altura;
                        img->dados = malloc(largura * altura * 3);

                        /* Copia os dados RGB do frame convertido para o
                         * buffer da imagem */
                        for (int y = 0; y < altura; y++)
                        {
                            memcpy(img->dados + y * largura * 3,
                                   pFrameRGB->data[0] +
                                       y * pFrameRGB->linesize[0],
                                   largura * 3);
                        }

                        list_ins_next(&imagens_em_memoria, NULL,
                                      img); /* Insere na lista */
                    }
                }
                frame_count++;
            }
        }
        av_packet_unref(&packet); /* Libera recursos do pacote */
    }

    // Inicializa a biblioteca FreeType
    if (FT_Init_FreeType(&ft_library))
    {
        fprintf(stderr, "Erro ao inicializar FreeType.\n");
        exit(EXIT_FAILURE);
    }

    // Carrega a fonte TrueType
    if (FT_New_Face(ft_library, "./font.ttf", 0, &ft_face))
    {
        fprintf(stderr, "Erro ao carregar a fonte.\n");
        exit(EXIT_FAILURE);
    }

    // Define o tamanho da fonte
    FT_Set_Pixel_Sizes(ft_face, 0,
                       24); /* Largura automática, altura de 24 pixels */

    const char *texto = "Uma mensagem bonita!";
    /* "🥳🧁🍰🎁🎂🎈🎺🎉🎊 📧〽︎🧿🌶︎🔋 😂❤︎😍🤣😊🥺🙏💕😭😘👍😅👏😁🔥💖"; */

    /* Após o loop: salva cada imagem armazenada em memória */
    ListElmt *elmt = list_head(&imagens_em_memoria);
    while (elmt != NULL)
    {
        ImagemRGB *img = (ImagemRGB *)list_data(elmt);

        desenhar_texto(img, 0, img->altura / 2, texto, 255, 0, 0);

        salvar_imagem_em_arquivo(img); /* Salva imagem .ppm */
        free(img->dados);              /* Libera dados RGB */

        /* Devolve o frame_id ao pool */
        free_frame(&frame_pool, img->id);

        elmt = list_next(elmt);
    }

    /* Liberação geral dos recursos */
    list_destroy(&imagens_em_memoria);
    list_destroy(&frame_pool);
    av_free(buffer);
    av_frame_free(&pFrame);
    av_frame_free(&pFrameRGB);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);
    sws_freeContext(sws_ctx); /* Libera contexto de conversão */

    // Libera os recursos da fonte e da biblioteca
    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_library);

    return 0;
}
