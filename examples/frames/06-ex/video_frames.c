#include <libavcodec/avcodec.h>   /* API de codecs da FFmpeg */
#include <libavformat/avformat.h> /* API de formatos/container de vídeo */
#include <libavutil/imgutils.h>   /* Funções utilitárias para imagens */
#include <libswscale/swscale.h> /* Biblioteca de conversão de formatos de pixel */

#include "frames.h" /* Gerenciador de slots/frame IDs */
#include <stdio.h>  /* Entrada e saída padrão */
#include <stdlib.h> /* Alocação de memória e controle geral */

/*
 * Inicializa um pool de frame_ids usando lista encadeada.
 * Cada frame_id é apenas um número inteiro, armazenado como ponteiro.
 * A função insere os inteiros de total-1 até 0 na lista.
 */
void init_frame_pool(List *pool, int total)
{
    list_init(pool,
              free); /* Inicializa a lista e define free como destruidor */
    for (int i = total - 1; i >= 0; i--)
    {
        int *f = malloc(sizeof(int)); /* Aloca um novo inteiro */
        *f = i;                       /* Atribui valor (frame_id) */
        list_ins_next(pool, NULL, f); /* Insere no início da lista */
    }
}

/*
 * Salva um frame RGB24 como imagem no formato PPM (formato simples e bruto).
 * Essa função grava o conteúdo da imagem RGB em disco.
 */
void salvar_frame(AVFrame *pFrameRGB, int largura, int altura, int id)
{
    char nome[64];
    snprintf(nome, sizeof(nome), "frame_%03d.ppm",
             id); /* Gera nome como "frame_000.ppm" */

    FILE *f = fopen(nome, "wb"); /* Abre o arquivo para escrita binária */
    fprintf(f, "P6\n%d %d\n255\n", largura,
            altura); /* Cabeçalho do formato PPM (tipo P6) */
    for (int y = 0; y < altura; y++)
    {
        /* Escreve os dados RGB da linha y */
        fwrite(pFrameRGB->data[0] + y * pFrameRGB->linesize[0], 1,
               largura * 3, f);
    }
    fclose(f);                   /* Fecha o arquivo */
    printf("Salvo: %s\n", nome); /* Informa que o arquivo foi salvo */
}

/*
 * Função principal do programa:
 * - Carrega o vídeo.
 * - Extrai quadros (frames).
 * - Converte quadros para RGB.
 * - Salva imagens a cada N frames.
 * - Gerencia slots usando o pool de frames.
 */
int main(int argc, char *argv[])
{
    /* Verifica se o nome do vídeo foi passado por parâmetro */
    if (argc < 2)
    {
        printf("Uso: %s arquivo.mp4\n", argv[0]);
        return -1;
    }

    AVFormatContext *pFormatCtx = NULL; /* Contexto do formato de vídeo */
    int videoStream;                    /* Índice do stream de vídeo */
    AVCodecContext *pCodecCtx = NULL;   /* Contexto do codec */
    AVFrame *pFrame = NULL,
            *pFrameRGB = NULL;         /* Frames: original e convertido */
    AVPacket packet;                   /* Pacote lido do vídeo */
    struct SwsContext *sws_ctx = NULL; /* Contexto de conversão de cores */

    /* Abre o arquivo de vídeo e lê informações de cabeçalho */
    avformat_open_input(&pFormatCtx, argv[1], NULL, NULL);
    avformat_find_stream_info(pFormatCtx, NULL);

    /* Encontra o índice do stream de vídeo */
    videoStream = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codecpar->codec_type ==
            AVMEDIA_TYPE_VIDEO)
            videoStream = i;

    if (videoStream == -1)
        return -1; /* Nenhum stream de vídeo encontrado */

    /* Localiza o codec apropriado para decodificar */
    const AVCodec *pCodec = avcodec_find_decoder(
        pFormatCtx->streams[videoStream]->codecpar->codec_id);

    /* Cria e configura contexto de decodificação */
    pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx,
                                  pFormatCtx->streams[videoStream]->codecpar);
    avcodec_open2(pCodecCtx, pCodec, NULL); /* Abre o codec */

    pFrame = av_frame_alloc();    /* Aloca frame original */
    pFrameRGB = av_frame_alloc(); /* Aloca frame convertido para RGB */

    /* Calcula o tamanho necessário para armazenar imagem RGB */
    int numBytes = av_image_get_buffer_size(
        AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

    /* Associa o buffer ao frame RGB */
    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer,
                         AV_PIX_FMT_RGB24, pCodecCtx->width,
                         pCodecCtx->height, 1);

    /* Inicializa o conversor de imagem (para RGB24) */
    sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                             pCodecCtx->pix_fmt, pCodecCtx->width,
                             pCodecCtx->height, AV_PIX_FMT_RGB24,
                             SWS_BILINEAR, NULL, NULL, NULL);

    List frame_pool;    /* Lista de frames disponíveis */
    List frames_salvos; /* Lista de frames salvos (para liberar depois) */
    init_frame_pool(&frame_pool, 10); /* Cria pool com 10 slots */
    list_init(&frames_salvos, free);  /* Inicializa lista de controle */

    int frame_count = 0; /* Contador total de frames processados */
    int intervalo = 100; /* Salvar um frame a cada 5 (ajustável) */

    /* Loop principal: leitura de pacotes de vídeo */
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        /* Verifica se o pacote é do stream de vídeo */
        if (packet.stream_index == videoStream)
        {
            avcodec_send_packet(pCodecCtx,
                                &packet); /* Envia pacote ao decoder */
            if (avcodec_receive_frame(pCodecCtx, pFrame) ==
                0) /* Recebe frame decodificado */
            {
                if (frame_count % intervalo ==
                    0) /* Salva 1 frame a cada 'intervalo' */
                {
                    printf("frame: %d\n", frame_count);

                    int id = alloc_frame(&frame_pool); /* Aloca um slot */
                    if (id != -1)
                    {
                        /* Converte para RGB usando o contexto SWS */
                        sws_scale(sws_ctx,
                                  (uint8_t const *const *)pFrame->data,
                                  pFrame->linesize, 0, pCodecCtx->height,
                                  pFrameRGB->data, pFrameRGB->linesize);

                        /* Salva a imagem do frame convertido */
                        salvar_frame(pFrameRGB, pCodecCtx->width,
                                     pCodecCtx->height, id);

                        /* Registra o ID usado para liberá-lo depois */
                        int *capturado = malloc(sizeof(int));
                        *capturado = id;
                        list_ins_next(&frames_salvos, NULL, capturado);
                    }
                }
                frame_count++; /* Incrementa número total de frames lidos */
            }
        }
        av_packet_unref(&packet); /* Libera memória do pacote lido */
    }

    /*
     * Após o processamento, libera todos os frame_ids que foram usados.
     * Isso simula a liberação de slots de processamento.
     */
    ListElmt *elmt = list_head(&frames_salvos);
    while (elmt != NULL)
    {
        int *id = (int *)list_data(elmt);
        free_frame(&frame_pool, *id); /* Libera slot */
        elmt = list_next(elmt);
    }

    /* Libera recursos e memória alocada */
    list_destroy(&frames_salvos);
    list_destroy(&frame_pool);
    av_free(buffer);
    av_frame_free(&pFrame);
    av_frame_free(&pFrameRGB);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}
