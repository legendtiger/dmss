#ifndef __DMSS_AVDecoder_H
#define __DMSS_AVDecoder_H
#include "ImportFFMpeg.h"
#include "Video.h"
#include <thread>
#include "IPlayer.h"

using namespace dmss::sdl;

namespace dmss
{
	namespace ffmpeg
	{
		class AVDecoder
		{
		private:
			static const int DELAYTIME = 10;
			static const int MAX_AUDIO_FRAME_SIZE = 192000;

		private:
			// ��¼ý�����
			AVFormatContext *m_pFormatCtx = NULL;

			// ��¼ת��Ŀ���ʽ
			AVPixelFormat m_fmt;

			// ��¼����Ƶͨ��
			int m_videoStream = -1;
			int m_audioStream = -1;

			// ��¼����Ƶ���뷽ʽ
			AVCodecContext *m_pVCodecCtx = NULL;
			AVCodecContext *m_pACodecCtx = NULL;

			// �ҵ���Ӧ������
			AVCodec *m_pVCodec = NULL;
			AVCodec *m_pACodec = NULL;

			// ԭʼ��Ƶ֡
			AVFrame* m_pFrame = NULL;

			// ת�����Ŀ����Ƶ֡
			AVFrame* m_pFrameRGB = NULL;
			// ת�������ƵĿ��
			AVFrame* m_pAudioFrame = NULL;
			uint8_t **m_pAudioBuffer = NULL;
			int m_audioSize;

			// ������Ҫ�Ļ�����
			uint8_t *m_buffer = NULL;

			// ͼ��ת������
			struct SwsContext *m_pSwsCtx = NULL;

			// ��Ƶת������
			struct SwrContext *m_pASwrCtx = NULL;

			Video & m_textuer;

			std::thread *m_pThreadDecoder;

			bool m_exitThred = false;
			bool m_threadExited = false;

		private:
			uint64_t m_dstChannelLayout = AV_CH_LAYOUT_STEREO;
			int m_nbSamples = 1024;
			AVSampleFormat m_dstSampleFmt = AV_SAMPLE_FMT_S16;
			int m_dstSampleRate = 44100;
			int m_dstChannels = 2;

		public:
			AVDecoder(Video &texture, AVPixelFormat fmt = AV_PIX_FMT_YUV420P);
			~AVDecoder();

		public:
			// ������
			bool Init(std::string url);

		private:
			void decoding(int start = 0);

			// ��ȡ��ʽ��Ϣ
			bool GetFormatContext(std::string url);

			// ��ȡ��
			int FindStream(AVMediaType type);

			bool InitVedio();
			bool InitAudio();

		private:
			// ����ý���ļ�
			static int Run(AVDecoder* decoder, int start);

			// ʱ��
			int m_duration;

			// ���벥��״̬
			PlayStatus	m_status;
		public:
			// ������Ƶ
			bool Play(int start);

			// ��ͣ
			void Pause();

			// ��������
			void Resume();

			// ��������
			void Stop();

			// ����״̬
			PlayStatus Status();

			// ���ڲ��ţ�
			bool IsPlaying();

			// ��ͣ��
			bool IsPuased();

			// ������
			bool IsStopped();

		private:
			void ExitThread();

		};
	}
}
#endif