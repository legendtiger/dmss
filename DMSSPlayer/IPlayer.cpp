#include "IPlayer.h"
#include "IUnpacking.h"
using namespace dmss;
using namespace dmss::ffmpeg;

IPlayer::IPlayer()
{

}


IPlayer::~IPlayer()
{
}

void IPlayer::AddUnpacker(IUnpacking* unpacker)
{
	m_pUnpacker = unpacker;
	if (m_pUnpacker == NULL)
	{
		throw "错误！必须传递IDecoder对象。";
	}
}

IUnpacking * IPlayer::Unpacker()
{ 
	return m_pUnpacker;
};

void IPlayer::SendPackage(PacketItem *packet)
{
	if (packet->packet.stream_index == m_pUnpacker->GetDecodeInfo().audioIndex)
	{
		m_audioQueue.enqueue(packet);
	}
	else if (packet->packet.stream_index == m_pUnpacker->GetDecodeInfo().videoIndex)
	{
		m_videoQueue.enqueue(packet);
	}
}