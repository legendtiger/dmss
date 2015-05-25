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
	// ��ý���ļ�
	if (avformat_open_input(&this->m_pFormatCtx, url.c_str(), NULL, NULL) != 0)
	{
		return false;
	}

	// ��ȡ����Ϣ
	if (avformat_find_stream_info(this->m_pFormatCtx, NULL) < 0)
	{
		return false;
	}

	// ��ӡý����Ϣ
	av_dump_format(m_pFormatCtx, 0, url.c_str(), 0);

	return true;
}

int AVDecoder::FindStream(AVMediaType type)
{
	// �ҵ���Ƶ����Ƶ��
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
	// �ҵ���Ƶ��/��Ƶ��
	this->m_videoStream = this->FindStream(AVMEDIA_TYPE_VIDEO);
	if (this->m_videoStream < 0)
	{
		return false;
	}

	// ȡ�ñ��뷽ʽ
	this->m_pVCodecCtx = this->m_pFormatCtx->streams[this->m_videoStream]->codec;

	// �ҵ���Ӧ������
	m_pVCodec = avcodec_find_decoder(m_pVCodecCtx->codec_id);
	if (m_pVCodec == NULL)
	{
		return false;
	}

	// �򿪽�����
	if (avcodec_open2(m_pVCodecCtx, m_pVCodec, NULL) < 0 )
	{
		return false;
	}

	// �������֡
	this->m_pFrame = av_frame_alloc();
	this->m_pFrameRGB = av_frame_alloc();
	if (m_pFrame == NULL || m_pFrameRGB == NULL)
	{
		return false;
	}

	// ������뻺����
	int numBytes = avpicture_get_size(this->m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);
	this->m_buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	avpicture_alloc((AVPicture *)m_pFrameRGB, m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);
	// ��ʼ�����Ŀ��֡
	//avpicture_fill((AVPicture *)m_pFrameRGB, m_buffer, m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);

	// ����ͼ��ת������
	m_pSwsCtx = sws_getContext(m_pVCodecCtx->width, m_pVCodecCtx->height,
		m_pVCodecCtx->pix_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height, m_fmt, SWS_BICUBIC, NULL, NULL, NULL);

	return true;
}

bool AVDecoder::InitAudio()
{
	// �ҵ���Ƶ��/��Ƶ��
	this->m_audioStream = this->FindStream(AVMEDIA_TYPE_AUDIO);
	if (this->m_audioStream < 0)
	{
		return false;
	}

	// ȡ�ñ��뷽ʽ
	this->m_pACodecCtx = this->m_pFormatCtx->streams[this->m_audioStream]->codec;

	// �ҵ���Ӧ������
	m_pACodec = avcodec_find_decoder(m_pACodecCtx->codec_id);
	if (m_pACodec == NULL)
	{
		return false;
	}

	// �򿪽�����
	if (avcodec_open2(m_pACodecCtx, m_pACodec, NULL) < 0)
	{
		return false;
	}

	// �������֡
	this->m_pAudioFrame = av_frame_alloc();
	if (m_pAudioFrame == NULL)
	{
		return false;
	}
	
	// ����ת����� 
	int in_channel_layout = m_pACodecCtx->channel_layout>0 ? m_pACodecCtx->channel_layout : av_get_default_channel_layout(m_pACodecCtx->channels);
	this->m_dstChannelLayout = in_channel_layout;
	int channels = av_get_channel_layout_nb_channels(m_pACodecCtx->channel_layout);
	this->m_dstChannels = m_pACodecCtx->channels > 0 ? m_pACodecCtx->channels : channels;
	this->m_dstSampleRate = m_pACodecCtx->sample_rate;
	
	// ����ת����� 
	m_pASwrCtx = swr_alloc_set_opts(m_pASwrCtx, m_dstChannelLayout, m_dstSampleFmt, m_dstSampleRate,
		in_channel_layout, m_pACodecCtx->sample_fmt, m_pACodecCtx->sample_rate, 0, NULL);
	if (m_pASwrCtx == NULL)
	{
		fprintf(stdout, "m_pASwrCtx ���ò���ʧ�ܣ�\n");
	}
	swr_init(m_pASwrCtx);

	// ����ת��buffer  
	av_samples_alloc_array_and_samples(&m_pAudioBuffer, &m_audioSize, m_dstChannels, m_dstSampleRate, m_dstSampleFmt, 0);
	fprintf(stdout, "channels = %d, sample_rate = %d, format = %d", m_dstChannels, m_dstSampleRate, m_dstSampleFmt);
	return true;
}

// ������
bool AVDecoder::Init(std::string url)
{
	// ��ȡ����Ϣ
	if (!GetFormatContext(url))
	{
		return false;
	}

	// �ҵ���Ƶ��/��Ƶ��
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
	// ֡���
	int frameFinished = 0;
	int frame_index = 1;

	// ���㲥����ʱ
	uint64_t startTime = av_gettime();
	fprintf(stdout, "startTime = %d\n", startTime);
	AVRational time_base = m_pFormatCtx->streams[m_videoStream]->time_base;
	AVRational time_base_q = { 1, AV_TIME_BASE };

	// �������ݰ�
	AVPacket packet;
	av_init_packet(&packet);

	av_seek_frame(m_pFormatCtx, -1, start*AV_TIME_BASE, AVSEEK_FLAG_FRAME);
	int startBase = start*AV_TIME_BASE;
	while (av_read_frame(m_pFormatCtx, &packet) >= 0 && !m_textuer.CanStop())
	{
		//û��pts���򵥼����д����� 
		if (packet.pts == AV_NOPTS_VALUE)
		{
			AVRational time_base1 = m_pFormatCtx->streams[m_videoStream]->time_base;
			//��֡���ʱ�䣬��λ(us)  
			int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(m_pFormatCtx->streams[m_videoStream]->r_frame_rate);

			//����pts��dts 
			packet.pts = (double)(frame_index*calc_duration) / (double)(av_q2d(time_base1)*AV_TIME_BASE);
			packet.dts = packet.pts;

			// ������ʱ������λ(s)
			packet.duration = (double)calc_duration / (double)(av_q2d(time_base1)*AV_TIME_BASE);
		}

		// ����Ƶ֡
		if (packet.stream_index == m_audioStream)
		{
			int total = packet.size;
			do
			{
				// ����
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
		// ����Ƶ֡
		else if (packet.stream_index == m_videoStream)
		{
			// ��¼֡���
			frame_index++;

			// ����
			avcodec_decode_video2(m_pVCodecCtx, m_pFrame, &frameFinished, &packet);

			// �õ�������Ƶ֡
			if (frameFinished)
			{
				// ת��Ϊָ����ʽ
				sws_scale(m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, m_pVCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);

				//���㱾֡����ʱ��
				// -����֡ʱ��
				int64_t pts_time = av_rescale_q(packet.pts, time_base, time_base_q);

				// �ӿ�ʼ���ž�����ʱ��
				int64_t pass_time = av_gettime() - startTime;

				int64_t diff_time = pts_time - startBase - pass_time;// -1000 * DELAYTIME;
				if (diff_time > 0)
				{
					// ��ʱ�󲥷�
					std::chrono::microseconds duration(diff_time);
					std::this_thread::sleep_for(duration);
				}

				// ���ű�֡��������ʾ
				m_textuer.Update(m_pFrameRGB->data[0], m_pFrameRGB->linesize[0]);
			}
		}

		av_free_packet(&packet);

		// ������Ƶ֡����ʱ�ó�CPU����������
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAYTIME));
	}

	m_textuer.DecodeFinished();
}


// ����ý���ļ�
int AVDecoder::Run(AVDecoder* decoder, int start)
{
	decoder->decoding(start);
	return 0;
}


// ������Ƶ
bool AVDecoder::Play(int start)
{
	// ���������߳�
	m_pThreadDecoder = new std::thread(AVDecoder::Run, this, start);
	m_pThreadDecoder->detach();

	return true;
}


// ��ͣ
void AVDecoder::Pause()
{
	this->m_status = PlayStatus::PUASE;
}


// ��������
void AVDecoder::Resume()
{
	this->m_status = PlayStatus::PLAYING;
}


// ��������
void AVDecoder::Stop()
{
	this->m_status = PlayStatus::STOP;
	ExitThread();
}


// ����״̬
PlayStatus AVDecoder::Status()
{
	return m_status;
}


// ���ڲ��ţ�
bool AVDecoder::IsPlaying()
{
	return this->m_status == PlayStatus::PLAYING;
}


// ��ͣ��
bool AVDecoder::IsPuased()
{
	return this->m_status == PlayStatus::PUASE;
}


// ������
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