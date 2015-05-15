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
			// �������
			IDecoder *m_pDecoder = NULL;

			// ����ý���
			SafeQueue<AVPacket> m_queue;

		public:
			// ��ý��
			virtual bool Open(std::string mediaUrl, int start = 0) = 0;

			// ���ţ�start��ʼʱ�䣬��λ��
			virtual bool Play(int start = 0) = 0;

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
			// ����������
			IDecoder * GetDecoder();

		private:
			// �ɽ������ص�������������ݸ����Ŷ���
			void SendPackage(AVPacket &packet);

			// �ɽ������ص������ݽ�����󵽲��Ŷ���
			void AddDecoder(IDecoder* decoder);

			// ָ��IDecoderΪ��Ԫ��
			friend class IDecoder;
		};
	}
}

#endif