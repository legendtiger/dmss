#include "AVDecoder.h"

using namespace dmss::ffmpeg;

AVDecoder::AVDecoder(Video &texture, AVPixelFormat fmt)
:m_textuer(texture)
, m_duration(0)
, m_status(PlayStatus::UNRIPE)
{
	this->m_fmt = fmt;

	avcodec_register_all();
	avfilter_register_all();
	av_register_all();
	avformat_network_init();
}


AVDecoder::~AVDecoder()
{
	av_frame_free(&m_pFrame);
	av_frame_free(&m_pFrameRGB);
	av_frame_free(&m_pAudioFrame);
	av_free(m_buffer);

	if (m_pAudioBuffer)
	{
		av_freep(&m_pAudioBuffer[0]);
	}		
	av_freep(&m_pAudioBuffer);

	swr_free(&m_pASwrCtx);

	avcodec_close(this->m_pACodecCtx);
	avcodec_close(this->m_pVCodecCtx);

	avformat_close_input(&this->m_pFormatCtx);
}

bool AVDecoder::GetFormatContext(std::string url)
{
	// 打开媒体文件
	if (avformat_open_input(&this->m_pFormatCtx, url.c_str(), NULL, NULL) != 0)
	{
		return false;
	}

	// 获取流信息
	if (avformat_find_stream_info(this->m_pFormatCtx, NULL) < 0)
	{
		return false;
	}

	// 打印媒体信息
	av_dump_format(m_pFormatCtx, 0, url.c_str(), 0);

	return true;
}

int AVDecoder::FindStream(AVMediaType type)
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

bool AVDecoder::InitVedio()
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

	avpicture_alloc((AVPicture *)m_pFrameRGB, m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);
	// 初始化填充目标帧
	//avpicture_fill((AVPicture *)m_pFrameRGB, m_buffer, m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);

	// 创建图形转换对象
	m_pSwsCtx = sws_getContext(m_pVCodecCtx->width, m_pVCodecCtx->height,
		m_pVCodecCtx->pix_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height, m_fmt, SWS_BICUBIC, NULL, NULL, NULL);

	return true;
}

bool AVDecoder::InitAudio()
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
	
	// 创建转码对象 
	int in_channel_layout = m_pACodecCtx->channel_layout>0 ? m_pACodecCtx->channel_layout : av_get_default_channel_layout(m_pACodecCtx->channels);
	this->m_dstChannelLayout = in_channel_layout;
	int channels = av_get_channel_layout_nb_channels(m_pACodecCtx->channel_layout);
	this->m_dstChannels = m_pACodecCtx->channels > 0 ? m_pACodecCtx->channels : channels;
	this->m_dstSampleRate = m_pACodecCtx->sample_rate;
	
	// 创建转码对象 
	m_pASwrCtx = swr_alloc_set_opts(m_pASwrCtx, m_dstChannelLayout, m_dstSampleFmt, m_dstSampleRate,
		in_channel_layout, m_pACodecCtx->sample_fmt, m_pACodecCtx->sample_rate, 0, NULL);
	if (m_pASwrCtx == NULL)
	{
		fprintf(stdout, "m_pASwrCtx 设置参数失败！\n");
	}
	swr_init(m_pASwrCtx);

	// 分配转码buffer  
	av_samples_alloc_array_and_samples(&m_pAudioBuffer, &m_audioSize, m_dstChannels, m_dstSampleRate, m_dstSampleFmt, 0);
	fprintf(stdout, "channels = %d, sample_rate = %d, format = %d", m_dstChannels, m_dstSampleRate, m_dstSampleFmt);
	return true;
}

// 播放流
bool AVDecoder::Init(std::string url)
{
	// 获取流信息
	if (!GetFormatContext(url))
	{
		return false;
	}

	// 找到视频流/音频流
	bool flag = false;
	if (InitVedio())
	{
		flag = true;
		m_textuer.InitVideo(m_pVCodecCtx->width, m_pVCodecCtx->height);
	}

	if (InitAudio())
	{
		flag = true;
		m_textuer.InitAudio(m_dstSampleRate, m_dstChannels, m_nbSamples);
	}

	if (!flag)
	{
		return false;
	}	

	this->m_status = PlayStatus::PREPARED;	

	return true;
}

void AVDecoder::decoding(int start)
{
	// 帧标记
	int frameFinished = 0;
	int frame_index = 1;

	// 计算播放延时
	uint64_t startTime = av_gettime();
	fprintf(stdout, "startTime = %d\n", startTime);
	AVRational time_base = m_pFormatCtx->streams[m_videoStream]->time_base;
	AVRational time_base_q = { 1, AV_TIME_BASE };

	// 解析数据包
	AVPacket packet;
	av_init_packet(&packet);

	av_seek_frame(m_pFormatCtx, -1, start*AV_TIME_BASE, AVSEEK_FLAG_FRAME);
	int startBase = start*AV_TIME_BASE;
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
				// 解码
				total -= avcodec_decode_audio4(m_pACodecCtx, m_pAudioFrame, &frameFinished, &packet);
				if (frameFinished != 0 && m_pASwrCtx)
				{
					int numSamplesOut = swr_convert(m_pASwrCtx, m_pAudioBuffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t**)m_pAudioFrame->data, m_pAudioFrame->nb_samples);
					if (numSamplesOut > 0)
					{
						int dst_bufsize = numSamplesOut * this->m_dstChannels * av_get_bytes_per_sample(this->m_dstSampleFmt);
						SDL_QueueAudio(1, m_pAudioBuffer[0], dst_bufsize);
					}					
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

				//计算本帧播放时间
				// -计算帧时间
				int64_t pts_time = av_rescale_q(packet.pts, time_base, time_base_q);

				// 从开始播放经过的时间
				int64_t pass_time = av_gettime() - startTime;

				int64_t diff_time = pts_time - startBase - pass_time;// -1000 * DELAYTIME;
				if (diff_time > 0)
				{
					// 延时后播放
					std::chrono::microseconds duration(diff_time);
					std::this_thread::sleep_for(duration);
				}

				// 播放本帧，更新显示
				m_textuer.Update(m_pFrameRGB->data[0], m_pFrameRGB->linesize[0]);
			}
		}

		av_free_packet(&packet);

		// 不是视频帧，延时让出CPU，继续解析
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAYTIME));
	}

	m_textuer.DecodeFinished();
}


// 解码媒体文件
int AVDecoder::Run(AVDecoder* decoder, int start)
{
	decoder->decoding(start);
	return 0;
}


// 播放视频
bool AVDecoder::Play(int start)
{
	// 创建解析线程
	m_pThreadDecoder = new std::thread(AVDecoder::Run, this, start);
	m_pThreadDecoder->detach();

	return true;
}


// 暂停
void AVDecoder::Pause()
{
	this->m_status = PlayStatus::PUASE;
}


// 继续播放
void AVDecoder::Resume()
{
	this->m_status = PlayStatus::PLAYING;
}


// 结束播放
void AVDecoder::Stop()
{
	this->m_status = PlayStatus::STOP;
	ExitThread();
}


// 播放状态
PlayStatus AVDecoder::Status()
{
	return m_status;
}


// 正在播放？
bool AVDecoder::IsPlaying()
{
	return this->m_status == PlayStatus::PLAYING;
}


// 暂停？
bool AVDecoder::IsPuased()
{
	return this->m_status == PlayStatus::PUASE;
}


// 结束？
bool AVDecoder::IsStopped()
{
	return this->m_status == PlayStatus::STOP;
}

void AVDecoder::ExitThread()
{
	m_exitThred = true;
	while (m_pThreadDecoder!=NULL)
	{
		if (m_threadExited)
		{
			break;
		}
			
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAYTIME));
	}
	m_pThreadDecoder = NULL;
	m_exitThred = false;
}