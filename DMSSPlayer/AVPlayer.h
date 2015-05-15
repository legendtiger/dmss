#ifndef __DMSS_AVPLAYER_H
#define __DMSS_AVPLAYER_H

#include "IPlayer.h"

namespace dmss
{
	namespace ffmpeg
	{
		class AVPlayer : public IPlayer
		{
		public:
			AVPlayer();
			~AVPlayer();

		private:
			// 播放状态
			PlayStatus m_status = PlayStatus::UNRIPE;

		public:
			// 打开媒体，start开始时间，单位秒
			bool Open(std::string mediaUrl, int start);

			// 播放
			bool Play();

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

		private:
			bool IsPrepared();
		};
	}
}

#endif