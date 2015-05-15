#ifndef __DMSS_QUEUE_H
#define __DMSS_QUEUE_H
#include "ImportSDL.h"
#include "ImportFFMpeg.h"

#include <queue>
namespace dmss
{
	namespace ffmpeg
	{
		class Queue
		{
		private:
			// �������
			std::queue<AVPacket *> m_queue;

			// ��¼���������ֽ���
			int m_totalSize;

			// �߳�ͬ��/����
			SDL_mutex *m_mutex;
			SDL_cond *m_cond;

		public:
			Queue();
			~Queue();

		public:
			void Push(AVPacket *packet);
			AVPacket *Pop();
		};
	}
}
#endif