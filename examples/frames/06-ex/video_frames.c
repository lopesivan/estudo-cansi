#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

#include "frames.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura para armazenar imagens RGB em memória */
typedef struct
{
    int id;
    int largura;
    int altura;
    uint8_t *dados; /* buffer RGB mallocado */
} ImagemRGB;

/* Inicializa um pool de frame_ids */
void init_frame_pool(List *pool, int total)
{
    list_init(pool, free);
    for (int i = total - 1; i >= 0; i--)
    {
        int *f = malloc(sizeof(int));
        *f = i;
        list_ins_next(pool, NULL, f);
    }
}

/* Função para salvar uma imagem RGB armazenada em memória */
void salvar_imagem_em_arquivo(const ImagemRGB *img)
{
    char nome[64];
    snprintf(nome, sizeof(nome), "imagem_%03d.ppm", img->id);

    FILE *f = fopen(nome, "wb");
    fprintf(f, "P6\n%d %d\n255\n", img->largura, img->altura);
    for (int y = 0; y < img->altura; y++)
    {
        fwrite(img->dados + y * img->largura * 3, 1, img->largura * 3, f);
    }
    fclose(f);
    printf("Imagem salva: %s\n", nome);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s arquivo.mp4\n", argv[0]);
        return -1;
    }

    AVFormatContext *pFormatCtx = NULL;
    int videoStream;
    AVCodecContext *pCodecCtx = NULL;
    AVFrame *pFrame = NULL, *pFrameRGB = NULL;
    AVPacket packet;
    struct SwsContext *sws_ctx = NULL;

    avformat_open_input(&pFormatCtx, argv[1], NULL, NULL);
    avformat_find_stream_info(pFormatCtx, NULL);

    videoStream = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codecpar->codec_type ==
            AVMEDIA_TYPE_VIDEO)
            videoStream = i;

    if (videoStream == -1)
        return -1;

    const AVCodec *pCodec = avcodec_find_decoder(
        pFormatCtx->streams[videoStream]->codecpar->codec_id);
    pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx,
                                  pFormatCtx->streams[videoStream]->codecpar);
    avcodec_open2(pCodecCtx, pCodec, NULL);

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    int largura = pCodecCtx->width;
    int altura = pCodecCtx->height;
    int numBytes =
        av_image_get_buffer_size(AV_PIX_FMT_RGB24, largura, altura, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes);

    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer,
                         AV_PIX_FMT_RGB24, largura, altura, 1);

    sws_ctx =
        sws_getContext(largura, altura, pCodecCtx->pix_fmt, largura, altura,
                       AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

    List frame_pool;
    List imagens_em_memoria;
    init_frame_pool(&frame_pool, 10);
    list_init(&imagens_em_memoria, free);

    /* Estima o número total de frames */
    int64_t total_frames = pFormatCtx->streams[videoStream]->nb_frames;
    if (total_frames == 0)
    {
        double duration = pFormatCtx->duration / (double)AV_TIME_BASE;
        AVRational framerate = pFormatCtx->streams[videoStream]->r_frame_rate;
        total_frames = (int64_t)(duration * av_q2d(framerate));
    }

    int intervalo = total_frames / 10;

    if (intervalo < 1)
        intervalo = 1; /* evita divisão por zero ou intervalo 0 */

    int frame_count = 0;
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == videoStream)
        {
            avcodec_send_packet(pCodecCtx, &packet);
            if (avcodec_receive_frame(pCodecCtx, pFrame) == 0)
            {
                if (frame_count % intervalo == 0)
                {
                    printf("frame: %d\n", frame_count);

                    int id = alloc_frame(&frame_pool);
                    if (id != -1)
                    {
                        sws_scale(sws_ctx,
                                  (uint8_t const *const *)pFrame->data,
                                  pFrame->linesize, 0, altura,
                                  pFrameRGB->data, pFrameRGB->linesize);

                        ImagemRGB *img = malloc(sizeof(ImagemRGB));
                        img->id = id;
                        img->largura = largura;
                        img->altura = altura;
                        img->dados = malloc(largura * altura * 3);

                        /* Copia os dados RGB do frame para memória própria */
                        for (int y = 0; y < altura; y++)
                        {
                            memcpy(img->dados + y * largura * 3,
                                   pFrameRGB->data[0] +
                                       y * pFrameRGB->linesize[0],
                                   largura * 3);
                        }

                        list_ins_next(&imagens_em_memoria, NULL, img);
                    }
                }
                frame_count++;
            }
        }
        av_packet_unref(&packet);
    }

    /* Após leitura completa, salva todas as imagens armazenadas */
    ListElmt *elmt = list_head(&imagens_em_memoria);
    while (elmt != NULL)
    {
        ImagemRGB *img = (ImagemRGB *)list_data(elmt);
        salvar_imagem_em_arquivo(img);
        free(img->dados);
        free_frame(&frame_pool, img->id);
        elmt = list_next(elmt);
    }

    /* Liberação geral de recursos */
    list_destroy(&imagens_em_memoria);
    list_destroy(&frame_pool);
    av_free(buffer);
    av_frame_free(&pFrame);
    av_frame_free(&pFrameRGB);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}
