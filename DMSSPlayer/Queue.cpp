#include "Queue.h"
using namespace dmss::ffmpeg;

Queue::Queue()
{
	m_mutex = SDL_CreateMutex();
	m_cond = SDL_CreateCond();
}


Queue::~Queue()
{
	SDL_DestroyMutex(m_mutex);
	SDL_DestroyCond(m_cond);
}

void Queue::Push(AVPacket *packet)
{
	//avcodec_decode_audio4
	SDL_LockMutex(m_mutex);
	this->m_queue.push(packet);
	this->m_totalSize += packet->size;
	SDL_CondSignal(m_cond);

	SDL_UnlockMutex(m_mutex);
}

AVPacket* Queue::Pop()
{
	AVPacket *packet = this->m_queue.front();
	this->m_queue.pop();
	return packet;
}