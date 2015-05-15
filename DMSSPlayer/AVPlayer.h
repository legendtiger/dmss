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
			// ����״̬
			PlayStatus m_status = PlayStatus::UNRIPE;

		public:
			// ��ý�壬start��ʼʱ�䣬��λ��
			bool Open(std::string mediaUrl, int start);

			// ����
			bool Play();

			// ��ͣ����
			void Pause();

			// �ָ�����
			void Resume();

			// ֹͣ����
			void Stop();

			// ���,duration���ʱ������λ��
			void QuickSeek(int duration);

			// ����״̬
			PlayStatus Status();

		private:
			bool IsPrepared();
		};
	}
}

#endif