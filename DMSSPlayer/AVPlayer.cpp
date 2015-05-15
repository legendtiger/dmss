#include "AVPlayer.h"
#include "IDecoder.h"

using namespace dmss::ffmpeg;

AVPlayer::AVPlayer()
{
}


AVPlayer::~AVPlayer()
{
}

// 打开媒体，start开始时间，单位秒
bool AVPlayer::Open(std::string mediaUrl, int start)
{
	// 初始化
	if (!GetDecoder()->Init(mediaUrl))
	{
		return false;
	}

	// 解包
	GetDecoder()->Decoding(start);

	// 设置状态
	m_status = PlayStatus::PREPARED;

	return true;
}

// 播放
bool AVPlayer::Play()
{
	if (IsPrepared())
	{
		
	}
	
}

// 暂停播放
void AVPlayer::Pause()
{
}

// 恢复播放
void AVPlayer::Resume()
{

}

// 停止播放
void AVPlayer::Stop()
{

}

// 快进,duration快进时长，单位秒
void AVPlayer::QuickSeek(int duration)
{

}

// 播放状态
PlayStatus AVPlayer::Status()
{
	return m_status;
}

bool AVPlayer::IsPrepared()
{
	return m_status == PlayStatus::PREPARED;
}
