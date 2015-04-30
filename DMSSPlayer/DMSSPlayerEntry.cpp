// DMSSPlayer.cpp : 定义控制台应用程序的入口点。
//
#include "SDLUtil.h"
#include "SDLWindow.h"
#include "SDLPicture.h"
#include "SDLButton.h"
#include "SDLVideoWnd.h"
#include "FFCommon.h"
#include <thread>

int flipThread(SDLWindow *pwnd)
{
	std::chrono::milliseconds dur(1);
	while (true)
	{
		pwnd->Flip();
		std::this_thread::sleep_for(dur);
	}
}

int playVedio(void *data)
{
	SDLVideoWnd *player = (SDLVideoWnd *)data;

	av_register_all();

	// 读取视频参数
	AVFormatContext *pFormatCtx = NULL;

	std::string filename = "k:/javastudy/test.avi";

	// Open video file
	if (avformat_open_input(&pFormatCtx, filename.c_str(), NULL, NULL) != 0)
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
	av_dump_format(pFormatCtx, 0, filename.c_str(), 0);

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

	player->InitVideo(pVCodecCtx->width, pVCodecCtx->height);

	int frameFinished = 0;
	AVPacket packet = { 0 };
	std::chrono::milliseconds duration(40);

	int frame_index = 1;
	uint64_t startTime = SDLUtil::CurrentTime();
	fprintf(stdout, "start time = %d\n", startTime);
	AVRational time_base = pFormatCtx->streams[videoStream]->time_base;
	AVRational time_base_q = { 1, AV_TIME_BASE };
	while (av_read_frame(pFormatCtx, &packet) >= 0)
	{
		//Simple Write PTS  
		if (packet.pts == AV_NOPTS_VALUE)
		{
			//Write PTS  
			AVRational time_base1 = pFormatCtx->streams[videoStream]->time_base;
			//Duration between 2 frames (us)  
			int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(pFormatCtx->streams[videoStream]->r_frame_rate);
			//Parameters  
			packet.pts = (double)(frame_index*calc_duration) / (double)(av_q2d(time_base1)*AV_TIME_BASE);
			packet.dts = packet.pts;
			packet.duration = (double)calc_duration / (double)(av_q2d(time_base1)*AV_TIME_BASE);
		}	

		// Is this a packet from the video stream?
		if (packet.stream_index == videoStream)
		{
			frame_index++;

			// Decode video frame
			avcodec_decode_video2(pVCodecCtx, pFrame, &frameFinished, &packet);
			
			// Did we get a video frame?
			if (frameFinished)
			{
				//fprintf(stdout, "pts = %d\n", pFrame->pts);
				//pFrame->pts = packet.pts;
				sws_scale(pSwsCtx, pFrame->data, pFrame->linesize, 0, pVCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
				
				//Important:Delay				
				int64_t pts_time = av_rescale_q(packet.pts, time_base, time_base_q)/1000;
				int64_t now_time = SDLUtil::CurrentTime() - startTime;
				if (pts_time > now_time)
				{
					std::chrono::milliseconds duration(pts_time - now_time);
					std::this_thread::sleep_for(duration);
				}
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

	return 0;
}

int main(int argc, char* argv[])
{
	SDLUtil::InitSDL();
	//SDLWindow first("First window", 300, 100);
	//first.show();
	//first.Move(10, 10);
	SDLWindow window("DMSS media player");
	window.show();
	//SDLPicture pic(window, "k:/javastudy/bkg.jpg");
	//SDLButton btn(window, "", "k:/javastudy/button.bmp", 500, 600, 200, 200);

	
	SDLVideoWnd player(window);
	std::thread thPlay(playVedio, &player);
	//SDL_CreateThread(playVedio, "hello", &player);	
	thPlay.detach();
	std::thread flip(flipThread, &window);
	flip.detach();
	window.EventLoop();
	SDLUtil::CloseSDL();
	return 0;
}

