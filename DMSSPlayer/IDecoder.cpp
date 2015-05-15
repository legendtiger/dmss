#include "IDecoder.h"
#include "IPlayer.h"
#include <sstream>

using namespace dmss::ffmpeg;

IDecoder::IDecoder(IPlayer* player)
:m_player(player)
{
	m_player->AddDecoder(this);
}


IDecoder::~IDecoder()
{
}

void IDecoder::Dispatch(AVPacket &packet)
{
	m_player->SendPackage(packet);
}

DecodeInfo &IDecoder::DecodeInfo()
{
	return m_decodeInfo;
}

void IDecoder::ResetDecodeInfo()
{
	if (m_decodeInfo.pFormatCtx)
	{
		avformat_close_input(&m_decodeInfo.pFormatCtx);
	}

	if (m_decodeInfo.pVCodecCtx)
	{
		avcodec_close(m_decodeInfo.pVCodecCtx);
	}

	if (m_decodeInfo.pACodecCtx)
	{
		avcodec_close(m_decodeInfo.pACodecCtx);
	}

	m_decodeInfo.Reset();
}

std::string DecodeInfo::Duration()
{
	std::ostringstream oss;
	oss.width(2);
	oss.fill('0');
	int hours = duration / 3600;
	int minutes = (duration % 3600) / 60;
	int seconds = duration % 60;
	oss << std::right << hours << ":" << minutes << ":" << seconds;
	return oss.str();
}

void DecodeInfo::Reset()
{
	url = "";

	// 记录媒体参数
	pFormatCtx = NULL;

	// 记录音视频编码上下文和解码器
	pVCodecCtx = NULL;
	videoIndex = -1;
	pACodecCtx = NULL;
	audioIndex = -1;

	// 记录媒体类型
	mediaType = UNKNOWN;

	// 记录媒体时长
	duration = 0;
}

