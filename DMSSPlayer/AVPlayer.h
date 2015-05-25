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
			// ���ٱ�־
			bool m_isExit = false;

			// ����״̬
			PlayStatus m_status = PlayStatus::UNRIPE;

			// ��Ƶ�����
			IDecoder *m_pADecoder = NULL;

			// ��Ƶ�����
			IDecoder *m_pVDecoder = NULL;

			// ��¼��ʼʱ��
			int64_t m_startTime;

			// ��¼��ͣʱ��
			int64_t m_pauseTime;

			// �����������Ŷ���
			AudioRender m_audioRender;

			// ��Ƶ���Ŷ���
			VideoRender *m_pVideoRender = NULL;

			// ��¼��һ֡��Ƶʱ��
			PacketItem m_preAudio;

			// �����������ڱ�����¼����һ֡��Ƶʱ��
			std::mutex m_mutex;

		public:
			// ��ý�壬start��ʼʱ�䣬��λ��
			bool Open(std::string mediaUrl, int start);

			// ����
			void Play();

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

			// ׼���ò���
			bool IsPrepared();

			// �̳�Destroy����
			void Destroy();

		protected:
			// ���������������̳�ʵ��
			IDecoder *CreateAudioDecoder(DecodeInfo info);
			IDecoder *CreateVideoDecoder(DecodeInfo info);

		private:
			// ���룬����
			void AudioDecoding();
			void VedioDecoding();

			// �ͷŽ������
			void Release();

			// ���»�׼ʱ��
			void UpdateTimebase(PacketItem &item);

			// ��ȡʱ���׼
			PacketItem GetTimebase();

			void ReleaseItem(PacketItem *item);
		private:
			// �����߳�ִ�з���
			static void PlayVideoFunction(AVPlayer *player);
			static void PlayAudioFunction(AVPlayer *player);
		};
	}
}

#endif