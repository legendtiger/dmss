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

	swr_free(&m_pASwrCtx);

	avcodec_close(this->m_pACodecCtx);
	avcodec_close(this->m_pVCodecCtx);

	avformat_close_input(&this->m_pFormatCtx);
}

bool FFAVDecoder::GetFormatContext(std::string url)
{
	// ����Ƶ�ļ�
	if (avformat_open_input(&this->m_pFormatCtx, url.c_str(), NULL, NULL) != 0)
	{
		return false;
	}

	// ��ȡ����Ϣ
	if (avformat_find_stream_info(this->m_pFormatCtx, NULL) < 0)
	{
		return false;
	}

	av_dump_format(m_pFormatCtx, 0, url.c_str(), 0);

	return true;
}

int FFAVDecoder::FindStream(AVMediaType type)
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

// ������
int FFAVDecoder::Play(std::string url)
{
	// ��ȡ����Ϣ
	if (!GetFormatContext(url))
	{
		return -1;
	}

	// �ҵ���Ƶ��/��Ƶ��
	this->m_videoStream = this->FindStream(AVMEDIA_TYPE_VIDEO);
	this->m_audioStream = this->FindStream(AVMEDIA_TYPE_AUDIO);
	if (this->m_videoStream < 0 || this->m_audioStream < 0)
	{
		return -1;
	}

	// ȡ�ñ��뷽ʽ
	this->m_pVCodecCtx = this->m_pFormatCtx->streams[this->m_videoStream]->codec;
	this->m_pACodecCtx = this->m_pFormatCtx->streams[this->m_audioStream]->codec;

	// �ҵ���Ӧ������
	m_pVCodec = avcodec_find_decoder(m_pVCodecCtx->codec_id);
	m_pACodec = avcodec_find_decoder(m_pACodecCtx->codec_id);
	if (m_pVCodec == NULL || m_pACodec == NULL)
	{
		return -1;
	}

	// �򿪽�����
	if (avcodec_open2(m_pVCodecCtx, m_pVCodec, NULL) < 0 ||
		avcodec_open2(m_pACodecCtx, m_pACodec, NULL) < 0)
	{
		return -1;
	}

	// �������֡
	this->m_pFrame = av_frame_alloc();
	this->m_pFrameRGB = av_frame_alloc();
	this->m_pAudioFrame = av_frame_alloc();
	if (m_pFrame == NULL || m_pFrameRGB == NULL | m_pAudioFrame == NULL)
	{
		return -1;
	}

	// ������뻺����
	int numBytes = avpicture_get_size(this->m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);
	this->m_buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	// ��ʼ�����Ŀ��֡
	avpicture_fill((AVPicture *)m_pFrameRGB, m_buffer, m_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height);

	// ����ͼ��ת������
	m_pSwsCtx = sws_getContext(m_pVCodecCtx->width, m_pVCodecCtx->height,
		m_pVCodecCtx->pix_fmt, m_pVCodecCtx->width, m_pVCodecCtx->height, m_fmt, SWS_BICUBIC, NULL, NULL, NULL);

	m_pAudioFrame->format = AV_SAMPLE_FMT_S16;
	m_pAudioFrame->sample_rate = m_pACodecCtx->sample_rate;
	m_pAudioFrame->channel_layout = av_get_default_channel_layout(m_pACodecCtx->channels);
	
	//Out Audio Param  
	uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
	int out_nb_samples = 1024;
	AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = m_pACodecCtx->sample_rate;//44100;
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	//Out Buffer Size  
	m_audioSize = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
	m_pAudioBuffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);

	int in_channel_layout = av_get_default_channel_layout(m_pACodecCtx->channels);
	m_pASwrCtx = swr_alloc();
	m_pASwrCtx = swr_alloc_set_opts(m_pASwrCtx, out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout, m_pACodecCtx->sample_fmt, m_pACodecCtx->sample_rate, 0, NULL);
	swr_init(m_pASwrCtx);
	
	m_textuer.InitVideo(m_pVCodecCtx->width, m_pVCodecCtx->height);
	//fprintf(stdout, "out_channels = %d\n", out_channels);
	m_textuer.InitAudio(out_sample_rate, out_sample_fmt, out_channels, out_nb_samples);

	// ���������߳�
	m_pThreadDecoder = new std::thread(FFAVDecoder::Run, this);
	m_pThreadDecoder->detach();

	return 0;
}

void FFAVDecoder::decoding()
{
	// ֡���
	int frameFinished = 0;
	int frame_index = 1;

	// ���㲥����ʱ
	uint64_t startTime = FFAVDecoder::CurrentTime();
	AVRational time_base = m_pFormatCtx->streams[m_videoStream]->time_base;
	AVRational time_base_q = { 1, AV_TIME_BASE };

	// �������ݰ�
	AVPacket packet;
	av_init_packet(&packet);
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
		if (packet.stream_index == m_videoStream)
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
				// ������ʾ
				m_textuer.UpdateFrame(m_pFrameRGB->data[0], m_pFrameRGB->linesize[0]);

				//������һ֡����ʱ��			
				int64_t pts_time = av_rescale_q(packet.pts, time_base, time_base_q) / 1000 - DELAYTIME;
				int64_t now_time = FFAVDecoder::CurrentTime() - startTime;
				if (pts_time > now_time)
				{
					std::chrono::milliseconds duration(pts_time - now_time);
					std::this_thread::sleep_for(duration);
				}
			}
		}
		else if (packet.stream_index == m_audioStream)
		{
			int total = packet.size;
			do
			{
				//fprintf(stdout, "size = %d\n", total);
				// ����
				total -= avcodec_decode_audio4(m_pACodecCtx, m_pFrame, &frameFinished, &packet);
				if (frameFinished != 0)
				{
					//fprintf(stdout, "size = %s\n", av_get_sample_fmt_name((AVSampleFormat)m_pFrame->format));
					//fprintf(stdout, "size = %d \/ %d = %d\n", m_pFrame->nb_samples, m_pFrame->linesize[0], m_pFrame->linesize[0] / m_pFrame->nb_samples);
					//if ()
					swr_config_frame(m_pASwrCtx, this->m_pAudioFrame, this->m_pFrame);
					if (0 == swr_convert_frame(m_pASwrCtx, this->m_pAudioFrame, this->m_pFrame))
					{
						fprintf(stdout, "size = %d \/ %d = %d\n", m_pAudioFrame->nb_samples, m_pAudioFrame->linesize[0], m_pAudioFrame->linesize[0] / m_pAudioFrame->nb_samples);
						SDL_QueueAudio(1, *m_pAudioFrame->extended_data, m_pAudioFrame->linesize[0]);
					}
					/*int numSamplesOut = swr_convert(m_pASwrCtx, &m_pAudioBuffer, MAX_AUDIO_FRAME_SIZE, 
						(const uint8_t**)m_pFrame->extended_data, m_pFrame->nb_samples);
					SDL_QueueAudio(1, m_pAudioBuffer, numSamplesOut*m_pFrame->channels);*/
				}
				else
				{
					total = -1;
				}
			} while (total > 0);
		}

		av_free_packet(&packet);

		// ������Ƶ֡����ʱ�ó�CPU����������
		std::this_thread::sleep_for(std::chrono::milliseconds(DELAYTIME));
	}

	m_textuer.DecodeFinished();
}


// ����ý���ļ�
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

// ������Ƶ����
bool FFAVDecoder::DecodingVedio()
{
	return false;
}