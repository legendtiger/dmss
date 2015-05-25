#ifndef __DMSS_AVPLAYER_H
#define __DMSS_AVPLAYER_H

#include "IPlayer.h"
#include "IDecoder.h"
#include "AudioRender.h"
#include "VideoRender.h"
#include <thread>

using namespace dmss::sdl;
namespace dmss
{
	namespace ffmpeg
	{
		class AVPlayer : public IPlayer
		{
		public:
			AVPlayer(VideoRender *observer);
			~AVPlayer();

		private:
			static const int PAUSEWAITTIME = 200;

		private:
			// 销毁标志
			bool m_isExit = false;

			// 播放状态
			PlayStatus m_status = PlayStatus::UNRIPE;

			// 视频解包器
			IDecoder *m_pADecoder = NULL;

			// 音频解包器
			IDecoder *m_pVDecoder = NULL;

			// 记录开始时间
			int64_t m_startTime;

			// 记录暂停时间
			int64_t m_pauseTime;

			// 定义声音播放对象
			AudioRender m_audioRender;

			// 视频播放对象
			VideoRender *m_pVideoRender = NULL;

			// 记录上一帧音频时间
			PacketItem m_preAudio;

			// 互斥量，用于保护记录的上一帧音频时间
			std::mutex m_mutex;

		public:
			// 打开媒体，start开始时间，单位秒
			bool Open(std::string mediaUrl, int start);

			// 播放
			void Play();

			// 暂停播放
			void Pause();

			// 恢复播放
			void Resume();

			// 停止播放
			void Stop();

			// 快进,duration快进时长，单位秒
			void QuickSeek(int duration);

			// 播放状态
			PlayStatus Status();

			// 准备好播放
			bool IsPrepared();

			// 继承Destroy方法
			void Destroy();

		protected:
			// 创建解码对象，子类继承实现
			IDecoder *CreateAudioDecoder(DecodeInfo info);
			IDecoder *CreateVideoDecoder(DecodeInfo info);

		private:
			// 解码，播放
			void AudioDecoding();
			void VedioDecoding();

			// 释放解码对象
			void Release();

			// 更新基准时间
			void UpdateTimebase(PacketItem &item);

			// 获取时间基准
			PacketItem GetTimebase();

			void ReleaseItem(PacketItem *item);
		private:
			// 播放线程执行方法
			static void PlayVideoFunction(AVPlayer *player);
			static void PlayAudioFunction(AVPlayer *player);
		};
	}
}

#endif