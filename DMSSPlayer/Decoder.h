#ifndef __DMSS_DECODER_H
#define __DMSS_DECODER_H
#include "IDecoder.h"
#include <thread>

namespace dmss
{
	namespace ffmpeg
	{
		class Decoder : public IDecoder
		{
			// Ĭ�Ͻ�ѹ���߼������λ����
			static const int SLEEPTIME = 10;

		public:
			Decoder(IPlayer* player, int interval = SLEEPTIME);
			~Decoder();

		private:
			// �����̱߳�־
			bool m_stopDecode = false;

			// ���߼��
			std::chrono::milliseconds m_interval;

			// ��ʼʱ��
			int m_startTime = 0;
			
		public:
			// ��ʼ��ý��
			bool Init(std::string url);

			// ��ָ��λ�ÿ�ʼ����, startָ��λ�ã���λΪ��			
			void Decoding(int start = 0);

			void Destroy();

		private:
			// ��ȡ��ʽ��Ϣ
			bool GetFormatContext(std::string url);

			// ��ȡ��
			int FindStream(AVMediaType type);

			// ��ý���ж�ȡ��
			void ReadPacket();

			void ExitThread();

			// ����̵߳��÷���
			static void DecodeThread(Decoder* decoder);
		};
	}
}

#endif