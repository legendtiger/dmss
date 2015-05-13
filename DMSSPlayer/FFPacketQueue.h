#ifndef __DMSS_FFPACKETQUEUE_H
#define __DMSS_FFPACKETQUEUE_H
#include "ImportSDL.h"
#include "ImportFFMpeg.h"

#include <queue>

class FFPacketQueue
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
	FFPacketQueue();
	~FFPacketQueue();

public:
	void Push(AVPacket *packet);
	AVPacket *Pop();
};

#endif