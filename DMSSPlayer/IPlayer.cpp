#include "IPlayer.h"
#include "IDecoder.h"
using namespace dmss;
using namespace dmss::ffmpeg;

IPlayer::IPlayer()
{

}


IPlayer::~IPlayer()
{
}

void IPlayer::AddDecoder(IDecoder* decoder)
{
	m_pDecoder = decoder;
	if (m_pDecoder == NULL)
	{
		throw "���󣡱��봫��IDecoder����";
	}
}

IDecoder * IPlayer::GetDecoder()
{ 
	return m_pDecoder; 
};

void IPlayer::SendPackage(AVPacket &packet)
{
	m_queue.enqueue(packet);
}