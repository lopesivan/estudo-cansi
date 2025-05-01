#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

#include "frames.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* Inicializa um pool de frame_ids (slots) usando list.c */
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

/* Salva um frame RGB24 como imagem PPM */
void salvar_frame(AVFrame *pFrameRGB, int largura, int altura, int id)
{
    char nome[64];
    snprintf(nome, sizeof(nome), "frame_%03d.ppm", id);

    FILE *f = fopen(nome, "wb");
    fprintf(f, "P6\n%d %d\n255\n", largura, altura);
    for (int y = 0; y < altura; y++)
    {
        fwrite(pFrameRGB->data[0] + y * pFrameRGB->linesize[0], 1,
               largura * 3, f);
    }
    fclose(f);
    printf("Salvo: %s\n", nome);
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

    int numBytes = av_image_get_buffer_size(
        AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer,
                         AV_PIX_FMT_RGB24, pCodecCtx->width,
                         pCodecCtx->height, 1);

    sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                             pCodecCtx->pix_fmt, pCodecCtx->width,
                             pCodecCtx->height, AV_PIX_FMT_RGB24,
                             SWS_BILINEAR, NULL, NULL, NULL);

    List frame_pool;
    List frames_salvos;
    init_frame_pool(&frame_pool, 10);
    list_init(&frames_salvos, free);

    int frame_count = 0;
    int intervalo = 5; /* mais responsivo para vídeos pequenos */

    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == videoStream)
        {
            avcodec_send_packet(pCodecCtx, &packet);
            if (avcodec_receive_frame(pCodecCtx, pFrame) == 0)
            {
                if (frame_count % intervalo == 0)
                {
                    int id = alloc_frame(&frame_pool);
                    if (id != -1)
                    {
                        sws_scale(sws_ctx,
                                  (uint8_t const *const *)pFrame->data,
                                  pFrame->linesize, 0, pCodecCtx->height,
                                  pFrameRGB->data, pFrameRGB->linesize);

                        salvar_frame(pFrameRGB, pCodecCtx->width,
                                     pCodecCtx->height, id);

                        int *capturado = malloc(sizeof(int));
                        *capturado = id;
                        list_ins_next(&frames_salvos, NULL, capturado);
                    }
                }
                frame_count++;
            }
        }
        av_packet_unref(&packet);
    }

    /* libera todos os frames salvos após o processamento */
    ListElmt *elmt = list_head(&frames_salvos);
    while (elmt != NULL)
    {
        int *id = (int *)list_data(elmt);
        free_frame(&frame_pool, *id);
        elmt = list_next(elmt);
    }

    /* libera recursos */
    list_destroy(&frames_salvos);
    list_destroy(&frame_pool);
    av_free(buffer);
    av_frame_free(&pFrame);
    av_frame_free(&pFrameRGB);
    avcodec_free_context(&pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}
