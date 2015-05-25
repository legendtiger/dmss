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
			// ý���
			AVPacket packet;

			// ����pts������ý�峤��(�ӿ�ʼλ�ÿ�ʼ����λ��΢�룩
			int64_t timestamp = 0;

			// һ֡����ʱ��
			int64_t duration = 0;

			// ��ǰʱ��
			int64_t time = 0;

			// �Ƿ������һ����
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
			// �������
			IUnpacking *m_pUnpacker = NULL;

		protected:
			// ����ý���
			SafeQueue<PacketItem *> m_audioQueue;
			SafeQueue<PacketItem *> m_videoQueue;

		public:
			// ��ý�壬start��ʼʱ�䣬��λ��
			virtual bool Open(std::string mediaUrl, int start = 0) = 0;

			// ����
			virtual void Play() = 0;

			// ��ͣ����
			virtual void Pause() = 0;

			// �ָ�����
			virtual void Resume() = 0;

			// ֹͣ����
			virtual void Stop() = 0;

			// ���,duration���ʱ������λ��
			virtual void QuickSeek(int duration) = 0;

			// ����״̬
			virtual PlayStatus Status() = 0;

		protected:
			// �������
			IUnpacking * Unpacker();

			// ���������������̳�ʵ��
			virtual IDecoder *CreateAudioDecoder(DecodeInfo info) = 0;
			virtual IDecoder *CreateVideoDecoder(DecodeInfo info) = 0;

		private:
			// �ɽ������ص�������������ݸ����Ŷ���
			void SendPackage(PacketItem* packet);

			// �ɽ������ص������ݽ�����󵽲��Ŷ���
			void AddUnpacker(IUnpacking* unpacker);

			// ָ��IDecoderΪ��Ԫ��
			friend class IUnpacking;
		};
	}
}

#endif