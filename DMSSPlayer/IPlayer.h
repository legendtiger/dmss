#ifndef __DMSS_IPLAYER_H
#define __DMSS_IPLAYER_H

#include "Object.h"
#include "ImportFFMpeg.h"
#include "SafeQueue.h"
#include <string>
#include "IDecoder.h"

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

		struct PacketItem
		{
			// 媒体包
			AVPacket packet;

			// 根据pts计算后的媒体长度(从开始位置开始，单位：微秒）
			int64_t timestamp = 0;

			// 一帧持续时间
			int64_t duration = 0;

			// 当前时刻
			int64_t time = 0;

			// 是否是最后一个包
			bool isLast = false;
		};

		class IPlayer :	public Object
		{
		public:
			/* no AV sync correction is done if below the minimum AV sync threshold */
			static const int AV_SYNC_THRESHOLD_MIN = 40000;

			/* AV sync correction is done if above the maximum AV sync threshold */
			static const int AV_SYNC_THRESHOLD_MAX = 100000;
		public:
			IPlayer();
			~IPlayer();

		private:
			// 解码对象
			IUnpacking *m_pUnpacker = NULL;

		protected:
			// 保存媒体包
			SafeQueue<PacketItem *> m_audioQueue;
			SafeQueue<PacketItem *> m_videoQueue;

		public:
			// 打开媒体，start开始时间，单位秒
			virtual bool Open(std::string mediaUrl, int start = 0) = 0;

			// 播放
			virtual void Play() = 0;

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
			// 解包对象
			IUnpacking * Unpacker();

			// 创建解码对象，子类继承实现
			virtual IDecoder *CreateAudioDecoder(DecodeInfo info) = 0;
			virtual IDecoder *CreateVideoDecoder(DecodeInfo info) = 0;

		private:
			// 由解码器回调，将解码包传递给播放对象
			void SendPackage(PacketItem* packet);

			// 由解码器回调，传递解码对象到播放对象
			void AddUnpacker(IUnpacking* unpacker);

			// 指定IDecoder为友元类
			friend class IUnpacking;
		};
	}
}

#endif