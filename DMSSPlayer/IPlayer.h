#ifndef __DMSS_IPLAYER_H
#define __DMSS_IPLAYER_H

#include "Object.h"
#include "ImportFFMpeg.h"
#include "SafeQueue.h"
#include <string>

namespace dmss
{
	namespace ffmpeg
	{
		enum PlayStatus
		{
			UNRIPE = -1,
			PREPARED,
			PLAYING,
			PUASE,
			STOP
		};

		class IDecoder;
		class IPlayer :	public Object
		{
		public:
			IPlayer();
			~IPlayer();

		private:
			// 解码对象
			IDecoder *m_pDecoder = NULL;

			// 保存媒体包
			SafeQueue<AVPacket> m_queue;

		public:
			// 打开媒体
			virtual bool Open(std::string mediaUrl, int start = 0) = 0;

			// 播放，start开始时间，单位秒
			virtual bool Play(int start = 0) = 0;

			// 暂停播放
			virtual void Pause() = 0;

			// 恢复播放
			virtual void Resume() = 0;

			// 停止播放
			virtual void Stop() = 0;

			// 快进,duration快进时长，单位秒
			virtual void QuickSeek(int duration) = 0;

			// 播放状态
			virtual PlayStatus Status() = 0;

		protected:
			// 解码器对象
			IDecoder * GetDecoder();

		private:
			// 由解码器回调，将解码包传递给播放对象
			void SendPackage(AVPacket &packet);

			// 由解码器回调，传递解码对象到播放对象
			void AddDecoder(IDecoder* decoder);

			// 指定IDecoder为友元类
			friend class IDecoder;
		};
	}
}

#endif