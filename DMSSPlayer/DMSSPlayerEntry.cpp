// DMSSPlayer.cpp : 定义控制台应用程序的入口点。
//
#include "SDLUtil.h"
#include "SDLWindow.h"
#include "SDLPicture.h"
#include "SDLButton.h"
#include "SDLVideoWnd.h"
#include "FFCommon.h"

int playVedio(void *data)
{
	SDLVideoWnd *player = (SDLVideoWnd *)data;
	// 读取视频参数
	AVFormatContext *pFormatCtx = NULL;

	// Open video file
	if (avformat_open_input(&pFormatCtx, "k:/javastudy/test.avi", NULL, NULL) != 0)
	{
		printf("Couldn't open file\n");
		return -1; // Couldn't open file
	}
	// Retrieve stream information
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("Couldn't find stream information\n");
		return -1; // Couldn't find stream information
	}

	// Dump information about file onto standard error
	av_dump_format(pFormatCtx, 0, "k:/javastudy/test.avi", 0);

	// Find the first video stream
	int videoStream = -1;
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}

	if (videoStream == -1)
	{
		printf("Didn't find a video stream\n");
		return -1; // Didn't find a video stream
	}

	// Get a pointer to the codec context for the video stream
	AVCodecContext *pVCodecCtx = pFormatCtx->streams[videoStream]->codec;

	// Find the decoder for the video stream
	AVCodec *pVCodec = avcodec_find_decoder(pVCodecCtx->codec_id);
	if (pVCodec == NULL)
	{
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found
	}
	// Open codec
	if (avcodec_open2(pVCodecCtx, pVCodec, NULL) < 0)
	{
		printf("Could not open codec!\n");
		return -1; // Could not open codec
	}

	// Allocate video frame
	AVFrame* pFrame = av_frame_alloc();

	// Allocate an AVFrame structure
	AVFrame* pFrameRGB = av_frame_alloc();
	if (pFrameRGB == NULL)
	{
		printf("pFrameRGB == NULL!\n");
		return -1;
	}

	// Determine required buffer size and allocate buffer
	int numBytes = avpicture_get_size(PIX_FMT_YUV420P, pVCodecCtx->width, pVCodecCtx->height);
	uint8_t *buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	// Assign appropriate parts of buffer to image planes in pFrameRGB
	// Note that pFrameRGB is an AVFrame, but AVFrame is a superset
	// of AVPicture
	avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_YUV420P, pVCodecCtx->width, pVCodecCtx->height);

	struct SwsContext *pSwsCtx = sws_getContext(pVCodecCtx->width, pVCodecCtx->height,
		pVCodecCtx->pix_fmt, pVCodecCtx->width, pVCodecCtx->height, PIX_FMT_YUV420P,
		SWS_BICUBIC, NULL, NULL, NULL);

	player->SetSize(pVCodecCtx->width, pVCodecCtx->height);

	int frameFinished;
	AVPacket packet;
	while (av_read_frame(pFormatCtx, &packet) >= 0)
	{
		// Is this a packet from the video stream?
		if (packet.stream_index == videoStream)
		{
			// Decode video frame
			avcodec_decode_video2(pVCodecCtx, pFrame, &frameFinished, &packet);

			// Did we get a video frame?
			if (frameFinished)
			{
				player->UpdateFrame(pFrameRGB->data[0], pFrameRGB->linesize[0]);
			}
		}
		av_free_packet(&packet);
	}

	// Free the RGB image
	av_free(buffer);
	av_free(pFrameRGB);

	// Free the YUV frame
	av_free(pFrame);

	// Close the codec
	avcodec_close(pVCodecCtx);

	// Close the video file
	avformat_close_input(&pFormatCtx);
}

int main(int argc, char* argv[])
{
	SDLUtil::InitSDL();
	//SDLWindow first("First window", 300, 100);
	//first.show();
	//first.Move(10, 10);
	SDLWindow window("DMSS media player");
	window.show();
	SDLPicture pic(window, "k:/javastudy/bkg.jpg");
	SDLButton btn(window, "", "k:/javastudy/button.bmp", 500, 600, 200, 200);

	av_register_all();
	SDLVideoWnd player(window);
	SDL_CreateThread(playVedio, "hello", &player);	

	window.EventLoop();
	SDLUtil::CloseSDL();
	return 0;
}

