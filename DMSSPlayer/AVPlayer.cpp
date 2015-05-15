#include "AVPlayer.h"
#include "IDecoder.h"

using namespace dmss::ffmpeg;

AVPlayer::AVPlayer()
{
}


AVPlayer::~AVPlayer()
{
}

// ��ý�壬start��ʼʱ�䣬��λ��
bool AVPlayer::Open(std::string mediaUrl, int start)
{
	// ��ʼ��
	if (!GetDecoder()->Init(mediaUrl))
	{
		return false;
	}

	// ���
	GetDecoder()->Decoding(start);

	// ����״̬
	m_status = PlayStatus::PREPARED;

	return true;
}

// ����
bool AVPlayer::Play()
{
	if (IsPrepared())
	{
		
	}
	
}

// ��ͣ����
void AVPlayer::Pause()
{
}

// �ָ�����
void AVPlayer::Resume()
{

}

// ֹͣ����
void AVPlayer::Stop()
{

}

// ���,duration���ʱ������λ��
void AVPlayer::QuickSeek(int duration)
{

}

// ����״̬
PlayStatus AVPlayer::Status()
{
	return m_status;
}

bool AVPlayer::IsPrepared()
{
	return m_status == PlayStatus::PREPARED;
}
