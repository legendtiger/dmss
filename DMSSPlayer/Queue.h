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
			// 定义队列
			std::queue<AVPacket *> m_queue;

			// 记录包数据总字节数
			int m_totalSize;

			// 线程同步/互斥
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