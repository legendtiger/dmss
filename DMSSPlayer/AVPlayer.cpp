#include "AVPlayer.h"
#include "IUnpacking.h"
#include "VideoDecoder.h"
#include "AudioDecoder.h"

using namespace dmss;
using namespace dmss::ffmpeg;

AVPlayer::AVPlayer(VideoRender *observer)
:m_pVideoRender(observer)
{
}


AVPlayer::~AVPlayer()
{
	Destroy();

	Release();

	Unpacker()->Destroy();
}

// ��ý�壬start��ʼʱ�䣬��λ��
bool AVPlayer::Open(std::string mediaUrl, int start)
{
	// ��ʼ��
	if (!Unpacker()->Init(mediaUrl))
	{
		return false;
	}

	Release();

	// �����������
	DecodeInfo &info = Unpacker()->GetDecodeInfo();
	m_pVDecoder = CreateVideoDecoder(info);
	if (m_pVDecoder)
	{
		m_pVideoRender->InitTexture(info.pVCodecCtx->width, info.pVCodecCtx->height);
		m_pVDecoder->AddListener(m_pVideoRender);
	}
	m_pADecoder = CreateAudioDecoder(info);

	// ���
	Unpacker()->Unpacking(start);

	// ����״̬
	m_status = PlayStatus::PREPARED;

	return true;
}

// ����
void AVPlayer::Play()
{
	if (IsPrepared())
	{
		m_startTime = av_gettime() + Unpacker()->GetDecodeInfo().pFormatCtx->start_time * AV_TIME_BASE;

		m_preAudio.time = av_gettime();
		DecodeInfo &info = Unpacker()->GetDecodeInfo();
		// ������Ƶ
		if (info.audioIndex >= 0)
		{
			std::thread playAudio(PlayAudioFunction, this);
			playAudio.detach();
		}

		// �ȴ�һ��
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		// ������Ƶ
		if (info.videoIndex >= 0)
		{
			std::thread playVideo(PlayVideoFunction, this);
			playVideo.detach();
		}
	}
}

// ��ͣ����
void AVPlayer::Pause()
{
	m_status == PlayStatus::PUASE;
}

// �ָ�����
void AVPlayer::Resume()
{
	m_status == PlayStatus::PLAYING;
}

// ֹͣ����
void AVPlayer::Stop()
{
	m_status == PlayStatus::STOP;
}

// ���,duration���ʱ������λ��
void AVPlayer::QuickSeek(int duration)
{

}

// ����״̬
PlayStatus AVPlayer::Status()
{
	return m_status;
}

bool AVPlayer::IsPrepared()
{
	return m_status == PlayStatus::PREPARED;
}

// �����߳�ִ�з���
void AVPlayer::PlayVideoFunction(AVPlayer *player)
{
	player->VedioDecoding();
	fprintf(stdout, "decoding end! \n");
}

// �����߳�ִ�з���
void AVPlayer::PlayAudioFunction(AVPlayer *player)
{
	player->AudioDecoding();
	fprintf(stdout, "decoding end! \n");
}

void AVPlayer::ReleaseItem(PacketItem *item)
{
	// �ͷ���Դ
	av_free_packet(&item->packet);
	delete item;
	item = NULL;
}

// ���룬����
void AVPlayer::VedioDecoding()
{
	DecodeInfo &info = Unpacker()->GetDecodeInfo();

	// ͬ������ʱ��
	int64_t diff_time = 0;
	while (true)
	{
		//	fprintf(stdout, "while ѭ��! ");
		// ֹͣ����
		if (m_isExit || m_status == PlayStatus::STOP)
		{
			break;
		}

		// ��ͣ,�ȴ�����
		if (m_status == PlayStatus::PUASE)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(PAUSEWAITTIME));
			continue;
		}

		PacketItem *item = m_videoQueue.dequeue();
		// �õ��ȴ�ʱ��
		if (item)
		{
			// �������
			if (item->isLast)
			{
				delete item;

				// ���ò���״̬
				m_status = PlayStatus::STOP;
				break;
			}

			PacketItem base = GetTimebase();
			int64_t pass_time = av_gettime() - base.time;
			diff_time = (item->timestamp - base.timestamp) - pass_time;
			fprintf(stdout, "pass_time = %d ", pass_time);
			fprintf(stdout, "base.timestamp = %d ", base.timestamp);
			fprintf(stdout, "item->timestamp = %d ", item->timestamp);
			fprintf(stdout, "item->duration = %d ", item->duration);
			fprintf(stdout, "diff_time = %d \n", diff_time);
			if (diff_time > 0)
			{
				/*
				fprintf(stdout, "duration = %d : ", item->duration);
				fprintf(stdout, "diff_time = %d : ", diff_time);
				fprintf(stdout, "pass_time = %d\n", pass_time);
				//*/

				// ��ʱ�󲥷�
				std::chrono::microseconds duration(diff_time);
				std::this_thread::sleep_for(duration);
			}			

			m_pVDecoder->Unpacking(item->packet);
			UpdateTimebase(*item);

			// �ͷ���Դ
			ReleaseItem(item);
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
	}
}

void AVPlayer::AudioDecoding()
{
	DecodeInfo &info = Unpacker()->GetDecodeInfo();

	m_preAudio.time = av_gettime();

	// ͬ������ʱ��
	int64_t diff_time = 0;
	while (true)
	{
		//	fprintf(stdout, "while ѭ��! ");
		// ֹͣ����
		if (m_isExit || m_status == PlayStatus::STOP)
		{
			break;
		}

		// ��ͣ,�ȴ�����
		if (m_status == PlayStatus::PUASE)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(PAUSEWAITTIME));
			continue;
		}

		PacketItem *item = m_audioQueue.dequeue();
		// �õ��ȴ�ʱ��
		if (item)
		{
			// �������
			if (item->isLast)
			{
				delete item;

				// ���ò���״̬
				m_status = PlayStatus::STOP;
				break;
			}

			PacketItem base = GetTimebase();
			int64_t pass_time = av_gettime() - base.time;
			diff_time = (item->timestamp - base.timestamp) - pass_time;
			
			if (diff_time > 0)
			{
				/*
				fprintf(stdout, "duration = %d : ", item->duration);
				fprintf(stdout, "diff_time = %d\n", diff_time);
				//*/
				// ��ʱ�󲥷�
				std::chrono::microseconds duration(diff_time);
				std::this_thread::sleep_for(duration);
			}
			else
			{
				/*
				fprintf(stdout, "duration = %d : ", item->duration);
				fprintf(stdout, "diff_time = %d\n", diff_time);
				//*/
			}

			m_pADecoder->Unpacking(item->packet);

			UpdateTimebase(*item);

			// �ͷ���Դ
			ReleaseItem(item);
		}
	}
}

void AVPlayer::UpdateTimebase(PacketItem &item)
{
	DecodeInfo &info = Unpacker()->GetDecodeInfo();
	std::lock_guard<std::mutex> gard(m_mutex);
	if (item.packet.stream_index == info.audioIndex || info.mediaType == MediaType::ONLEVIDEO)
	{
		m_preAudio.timestamp = item.timestamp;
		m_preAudio.time = av_gettime();
		m_preAudio.duration = item.duration;
	}
}

PacketItem AVPlayer::GetTimebase()
{
	std::lock_guard<std::mutex> gard(m_mutex);
	
	return m_preAudio;
}

void AVPlayer::Destroy()
{
	m_isExit = true;
}

IDecoder *AVPlayer::CreateAudioDecoder(DecodeInfo info)
{
	if (info.audioIndex < 0)
	{
		return NULL;
	}

	AudioLayout layout = AudioDecoder::CreateAudioLayout(AV_SAMPLE_FMT_S16);
	IDecoder *decoder = new AudioDecoder(info, layout);

	// ��ʼ����Ƶ����
	m_audioRender.InitAudio(info.pACodecCtx->sample_rate, info.pACodecCtx->channels, AudioRender::DEFAULTFREQUENCY);

	// ������Ƶ����
	decoder->AddListener(&m_audioRender);
	return decoder;
}

IDecoder *AVPlayer::CreateVideoDecoder(DecodeInfo info)
{
	if (info.videoIndex < 0)
	{
		return NULL;
	}
	return new VideoDecoder(info);
}

void AVPlayer::Release()
{
	if (m_pVDecoder != NULL)
	{
		delete m_pVDecoder;
		m_pVDecoder = NULL;
	}

	if (m_pADecoder != NULL)
	{
		delete m_pADecoder;
		m_pADecoder = NULL;
	}
}

