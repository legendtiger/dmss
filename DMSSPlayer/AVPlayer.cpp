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

// 打开媒体，start开始时间，单位秒
bool AVPlayer::Open(std::string mediaUrl, int start)
{
	// 初始化
	if (!Unpacker()->Init(mediaUrl))
	{
		return false;
	}

	Release();

	// 创建解码对象
	DecodeInfo &info = Unpacker()->GetDecodeInfo();
	m_pVDecoder = CreateVideoDecoder(info);
	if (m_pVDecoder)
	{
		m_pVideoRender->InitTexture(info.pVCodecCtx->width, info.pVCodecCtx->height);
		m_pVDecoder->AddListener(m_pVideoRender);
	}
	m_pADecoder = CreateAudioDecoder(info);

	// 解包
	Unpacker()->Unpacking(start);

	// 设置状态
	m_status = PlayStatus::PREPARED;

	return true;
}

// 播放
void AVPlayer::Play()
{
	if (IsPrepared())
	{
		m_startTime = av_gettime() + Unpacker()->GetDecodeInfo().pFormatCtx->start_time * AV_TIME_BASE;

		m_preAudio.time = av_gettime();
		DecodeInfo &info = Unpacker()->GetDecodeInfo();
		// 播放音频
		if (info.audioIndex >= 0)
		{
			std::thread playAudio(PlayAudioFunction, this);
			playAudio.detach();
		}

		// 等待一下
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		// 播放视频
		if (info.videoIndex >= 0)
		{
			std::thread playVideo(PlayVideoFunction, this);
			playVideo.detach();
		}
	}
}

// 暂停播放
void AVPlayer::Pause()
{
	m_status == PlayStatus::PUASE;
}

// 恢复播放
void AVPlayer::Resume()
{
	m_status == PlayStatus::PLAYING;
}

// 停止播放
void AVPlayer::Stop()
{
	m_status == PlayStatus::STOP;
}

// 快进,duration快进时长，单位秒
void AVPlayer::QuickSeek(int duration)
{

}

// 播放状态
PlayStatus AVPlayer::Status()
{
	return m_status;
}

bool AVPlayer::IsPrepared()
{
	return m_status == PlayStatus::PREPARED;
}

// 播放线程执行方法
void AVPlayer::PlayVideoFunction(AVPlayer *player)
{
	player->VedioDecoding();
	fprintf(stdout, "decoding end! \n");
}

// 播放线程执行方法
void AVPlayer::PlayAudioFunction(AVPlayer *player)
{
	player->AudioDecoding();
	fprintf(stdout, "decoding end! \n");
}

void AVPlayer::ReleaseItem(PacketItem *item)
{
	// 释放资源
	av_free_packet(&item->packet);
	delete item;
	item = NULL;
}

// 解码，播放
void AVPlayer::VedioDecoding()
{
	DecodeInfo &info = Unpacker()->GetDecodeInfo();

	// 同步播放时间
	int64_t diff_time = 0;
	while (true)
	{
		//	fprintf(stdout, "while 循环! ");
		// 停止播放
		if (m_isExit || m_status == PlayStatus::STOP)
		{
			break;
		}

		// 暂停,等待播放
		if (m_status == PlayStatus::PUASE)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(PAUSEWAITTIME));
			continue;
		}

		PacketItem *item = m_videoQueue.dequeue();
		// 得到等待时间
		if (item)
		{
			// 播放完毕
			if (item->isLast)
			{
				delete item;

				// 设置播放状态
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

				// 延时后播放
				std::chrono::microseconds duration(diff_time);
				std::this_thread::sleep_for(duration);
			}			

			m_pVDecoder->Unpacking(item->packet);
			UpdateTimebase(*item);

			// 释放资源
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

	// 同步播放时间
	int64_t diff_time = 0;
	while (true)
	{
		//	fprintf(stdout, "while 循环! ");
		// 停止播放
		if (m_isExit || m_status == PlayStatus::STOP)
		{
			break;
		}

		// 暂停,等待播放
		if (m_status == PlayStatus::PUASE)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(PAUSEWAITTIME));
			continue;
		}

		PacketItem *item = m_audioQueue.dequeue();
		// 得到等待时间
		if (item)
		{
			// 播放完毕
			if (item->isLast)
			{
				delete item;

				// 设置播放状态
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
				// 延时后播放
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

			// 释放资源
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

	// 初始化音频参数
	m_audioRender.InitAudio(info.pACodecCtx->sample_rate, info.pACodecCtx->channels, AudioRender::DEFAULTFREQUENCY);

	// 监听音频数据
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

