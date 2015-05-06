#ifndef __DMSS_FFPACKETQUEUE_H
#define __DMSS_FFPACKETQUEUE_H
#include "SDLCommon.h"
#include "FFCommon.h"

#include <queue>

class FFPacketQueue
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
	FFPacketQueue();
	~FFPacketQueue();

public:
	void Push(AVPacket *packet);
	AVPacket *Pop();
};

#endif