#include "FFAVDecoder.h"
#include <sys/timeb.h>


FFAVDecoder::FFAVDecoder(SDLVideoWnd &texture, AVPixelFormat fmt)
:m_textuer(texture)
{
	this->m_fmt = fmt;

	av_register_all();	
}


FFAVDecoder::~FFAVDecoder()
{
	av_frame_free(&m_pFrame);
	av_frame_free(&m_pFrameRGB);
	av_frame_free(&m_pAudioFrame);
	av_free(m_buffer);

	if (m_pAudioBuffer)
	{
		//av_freep(&m_pAudioBuffer[0]);
	}		
	//av_freep(&m_pAudioBuffer);

	swr_free(&m_pASwrCtx);

	avcodec_close(this->m_pACodecCtx);
	avcodec_close(this->m_pVCodecCtx);

	avformat_close_input(&this->m_pFormatCtx);
}

bool FFAVDecoder::GetFormatContext(std::string url)
{
	// 打开视频文件
	if (avformat_open_input(&this->m_pFormatCtx, url.c_str(), NULL, NULL) != 0)
	{
		return false;
	}

	// 获取流信息
	if (avformat_find_stream_info(this->m_pFormatCtx, NULL) < 0)
	{
		return false;
	}

	av_dump_format(m_pFormatCtx, 0, url.c_str(), 0);

	return true;
}

int FFAVDecoder::FindStream(AVMediaType type)
{
	// 找到视频和音频流
	for (unsigned int i = 0; i < this->m_pFormatCtx->nb_streams; i++)
	{
		if (this->m_pFormatCtx->streams[i]->codec->codec_type == type)
		{
			return i;			
		}
	}

	return -1;
}

bool FFAVDecoder::InitVedio()
{
	// 找到视频流/音频流
	this->m_videoStream = this->FindStream(AVMEDIA_TYPE_VIDEO);
	if (this->m_videoStream < 0)
	{
		return false;
	}

	// 取得编码方式
	this->m_pVCodecCtx = this->m_pFormatCtx->streams[this->m_videoStream]->codec;

	// 找到对应解码器
	m_pVCodec = avcodec_find_decoder(m_pVCodecCtx->codec_id);
	if (m_pVCodec == NULL)
	{
		return false;
	}

	// 打开解码器
	if (avcodec_open2(m_pVCodecCtx, m_pVCodec, NULL) < 0 )
	{
		return false;
	}

	// 分配解码帧
	this->m_pFrame = av_frame_alloc();
	this->m_pFrameRGB = av_frame_alloc();
	if (m_pFrame == NULL || m_pFrameRGB == NULL)
	{
		return false;
	}

	// 分配解码缓冲区
	int numBytes = avpicture_get_size(this->m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);
	this->m_buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	// 初始化填充目标帧
	avpicture_fill((AVPicture *)m_pFrameRGB, m_buffer, m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);

	// 创建图形转换对象
	m_pSwsCtx = sws_getContext(m_pVCodecCtx->width, m_pVCodecCtx->height,
		m_pVCodecCtx->pix_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height, m_fmt, SWS_BICUBIC, NULL, NULL, NULL);

	return true;
}

bool FFAVDecoder::InitAudio()
{
	// 找到视频流/音频流
	this->m_audioStream = this->FindStream(AVMEDIA_TYPE_AUDIO);
	if (this->m_audioStream < 0)
	{
		return false;
	}

	// 取得编码方式
	this->m_pACodecCtx = this->m_pFormatCtx->streams[this->m_audioStream]->codec;

	// 找到对应解码器
	m_pACodec = avcodec_find_decoder(m_pACodecCtx->codec_id);
	if (m_pACodec == NULL)
	{
		return false;
	}

	// 打开解码器
	if (avcodec_open2(m_pACodecCtx, m_pACodec, NULL) < 0)
	{
		return false;
	}

	// 分配解码帧
	this->m_pAudioFrame = av_frame_alloc();
	if (m_pAudioFrame == NULL)
	{
		return false;
	}
	
	//Destination Audio Param 
	this->m_dstChannelLayout = m_pACodecCtx->channel_layout;//av_get_default_channel_layout(m_pACodecCtx->channels);
	int channels = av_get_channel_layout_nb_channels(m_pACodecCtx->channel_layout);
	this->m_dstChannels = m_pACodecCtx->channels > 0 ? m_pACodecCtx->channels : channels;
	this->m_dstSampleRate = m_pACodecCtx->sample_rate;
	int in_channel_layout = m_pACodecCtx->channel_layout>0 ? m_pACodecCtx->channel_layout : av_get_default_channel_layout(m_pACodecCtx->channels);

	m_pASwrCtx = swr_alloc_set_opts(m_pASwrCtx, m_dstChannelLayout, m_dstSampleFmt, m_dstSampleRate,
		m_pACodecCtx->channel_layout, m_pACodecCtx->sample_fmt, m_pACodecCtx->sample_rate, 0, NULL);
	if (m_pASwrCtx == NULL)
	{
		fprintf(stdout, "m_pASwrCtx 设置参数失败！\n");
	}
	swr_init(m_pASwrCtx);

	//Out Buffer Size  
	av_samples_alloc_array_and_samples(&m_pAudioBuffer, &m_audioSize, m_dstChannels, m_dstSampleRate, m_dstSampleFmt, 0);

	return true;
}

// 播放流
int FFAVDecoder::Play(std::string url)
{
	// 获取流信息
	if (!GetFormatContext(url))
	{
		return -1;
	}

	// 找到视频流/音频流
	if (!(InitAudio() | InitVedio()))
	{
		return -1;
	}

	m_textuer.InitVideo(m_pVCodecCtx->width, m_pVCodecCtx->height);
	m_textuer.InitAudio(m_dstSampleRate, m_dstSampleFmt, m_dstChannels, m_nbSamples);

	// 创建解析线程
	m_pThreadDecoder = new std::thread(FFAVDecoder::Run, this);
	m_pThreadDecoder->detach();

	return 0;
}

void FFAVDecoder::decoding()
{
	// 帧标记
	int frameFinished = 0;
	int frame_index = 1;

	// 计算播放延时
	uint64_t startTime = FFAVDecoder::CurrentTime();
	AVRational time_base = m_pFormatCtx->streams[m_videoStream]->time_base;
	AVRational time_base_q = { 1, AV_TIME_BASE };

	// 解析数据包
	AVPacket packet;
	av_init_packet(&packet);
	while (av_read_frame(m_pFormatCtx, &packet) >= 0 && !m_textuer.CanStop())
	{
		//没有pts，简单计算后写入包中 
		if (packet.pts == AV_NOPTS_VALUE)
		{
			AVRational time_base1 = m_pFormatCtx->streams[m_videoStream]->time_base;
			//两帧间隔时间，单位(us)  
			int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(m_pFormatCtx->streams[m_videoStream]->r_frame_rate);

			//保存pts和dts 
			packet.pts = (double)(frame_index*calc_duration) / (double)(av_q2d(time_base1)*AV_TIME_BASE);
			packet.dts = packet.pts;

			// 保存总时长，单位(s)
			packet.duration = (double)calc_duration / (double)(av_q2d(time_base1)*AV_TIME_BASE);
		}

		// 是音频帧
		if (packet.stream_index == m_audioStream)
		{
			int total = packet.size;
			do
			{
				//fprintf(stdout, "size = %d\n", total);
				//fprintf(stdout, "channel_layout = %d\n", m_pFrame->channel_layout);
				// 解码
				total -= avcodec_decode_audio4(m_pACodecCtx, m_pFrame, &frameFinished, &packet);
				if (frameFinished != 0)
				{
					int numSamplesOut = swr_convert(m_pASwrCtx, m_pAudioBuffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t**)m_pFrame->extended_data, m_pFrame->nb_samples);

					int dst_bufsize = numSamplesOut * m_pACodecCtx->channels * av_get_bytes_per_sample(m_pACodecCtx->sample_fmt);
					SDL_QueueAudio(1, m_pAudioBuffer[0], dst_bufsize);
				}
				else
				{
					total = -1;
				}
			} while (total > 0);
		}
		// 是视频帧
		else if (packet.stream_index == m_videoStream)
		{
			// 记录帧序号
			frame_index++;

			// 解码
			avcodec_decode_video2(m_pVCodecCtx, m_pFrame, &frameFinished, &packet);

			// 得到完整视频帧
			if (frameFinished)
			{
				// 转换为指定格式
				sws_scale(m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, m_pVCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);
				// 更新显示
				m_textuer.UpdateFrame(m_pFrameRGB->data[0], m_pFrameRGB->linesize[0]);

				//计算下一帧播放时间			
				int64_t pts_time = av_rescale_q(packet.pts, time_base, time_base_q) / 1000 - DELAYTIME;
				int64_t now_time = FFAVDecoder::CurrentTime() - startTime;
				if (pts_time > now_time)
				{
					std::chrono::milliseconds duration(pts_time - now_time);
					std::this_thread::sleep_for(duration);
				}
			}
		}

		av_free_packet(&packet);

		// 不是视频帧，延时让出CPU，继续解析
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAYTIME));
	}

	m_textuer.DecodeFinished();
}


// 解码媒体文件
int FFAVDecoder::Run(FFAVDecoder* decoder)
{
	decoder->decoding();
	return 0;
}

uint64_t FFAVDecoder::CurrentTime()
{
	timeb t;
	ftime(&t);
	return 1000 * t.time + t.millitm;
}

// 解析视频数据
bool FFAVDecoder::DecodingVedio()
{
	return false;
}
