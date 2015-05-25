#ifndef __DMSS_UNPACKER_H
#define __DMSS_UNPACKER_H
#include "IUnpacking.h"
#include <thread>

namespace dmss
{
	namespace ffmpeg
	{
		class Unpacker : public IUnpacking
		{
			// Ĭ�Ͻ�ѹ���߼������λ����
			static const int SLEEPTIME = 10;

		public:
			Unpacker(IPlayer* player, int interval = SLEEPTIME);
			~Unpacker();

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
			void Unpacking(int start = 0);

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
			static void DecodeThread(Unpacker* unpacker);
		};
	}
}

#endif