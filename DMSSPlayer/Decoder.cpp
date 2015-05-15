#include "Decoder.h"

using namespace dmss::ffmpeg;

Decoder::Decoder(IPlayer* player, int interval)
:IDecoder(player), m_interval(interval)
{
	avcodec_register_all();
	avfilter_register_all();
	av_register_all();
	avformat_network_init();
}


Decoder::~Decoder()
{
}

// 初始化媒体
bool Decoder::Init(std::string url)
{
	ResetDecodeInfo();

	return GetFormatContext(url);
}

// 从指定位置开始解码, start指定位置，单位为秒			
void Decoder::Decoding(int start)
{
	// 退出已有线程
	ExitThread();

	// 开启解码线程
	m_startTime = start;
	std::thread thread(DecodeThread, this);
	thread.detach();
}

void Decoder::ReadPacket()
{
	// 起始位置
	int startBase = m_startTime * AV_TIME_BASE;	

	// 初始化数据包
	AVPacket packet;
	av_init_packet(&packet);

	// 跳到解包位置
	av_seek_frame(m_decodeInfo.pFormatCtx, -1, startBase, AVSEEK_FLAG_FRAME);

	// 解包
	while (av_read_frame(m_decodeInfo.pFormatCtx, &packet) >= 0)
	{
		// 停止读数据包
		if (m_stopDecode)
		{
			break;
		}

		// 将解出的数据包传递到播放对象
		Dispatch(packet);

		std::this_thread::sleep_for(m_interval);
	}

	m_stopDecode = false;
}

bool Decoder::GetFormatContext(std::string url)
{
	m_decodeInfo.url = url;

	// 打开媒体文件
	if (avformat_open_input(&m_decodeInfo.pFormatCtx, url.c_str(), NULL, NULL) != 0)
	{
		return false;
	}

	// 获取流信息
	if (avformat_find_stream_info(m_decodeInfo.pFormatCtx, NULL) < 0)
	{
		return false;
	}	

	// 媒体长度
	m_decodeInfo.duration = m_decodeInfo.pFormatCtx->duration / AV_TIME_BASE;

	// 视频流
	m_decodeInfo.videoIndex = FindStream(AVMEDIA_TYPE_VIDEO);
	if (m_decodeInfo.videoIndex != -1)
	{
		m_decodeInfo.pVCodecCtx = m_decodeInfo.pFormatCtx->streams[m_decodeInfo.videoIndex]->codec;		
		m_decodeInfo.mediaType = MediaType::ONLEVIDEO;
	}

	// 音频流
	m_decodeInfo.audioIndex = FindStream(AVMEDIA_TYPE_AUDIO);
	if (m_decodeInfo.videoIndex != -1)
	{
		m_decodeInfo.pACodecCtx = m_decodeInfo.pFormatCtx->streams[m_decodeInfo.audioIndex]->codec;
		m_decodeInfo.mediaType = MediaType(0 | m_decodeInfo.mediaType | MediaType::ONLEAUDIO);
	}	

	// 打印媒体信息
	av_dump_format(m_decodeInfo.pFormatCtx, 0, url.c_str(), 0);
	return true;
}

// 获取流
int Decoder::FindStream(AVMediaType type)
{
	// 找到视频和音频流
	for (unsigned int i = 0; i < m_decodeInfo.pFormatCtx->nb_streams; i++)
	{
		if (m_decodeInfo.pFormatCtx->streams[i]->codec->codec_type == type)
		{
			return i;
		}
	}

	return -1;
}

void Decoder::DecodeThread(Decoder* decoder)
{
	decoder->ReadPacket();
}

void Decoder::Destroy()
{
	// 退出线程
	ExitThread();

	// 释放资源
	ResetDecodeInfo();
}

void Decoder::ExitThread()
{
	// 线程没有启动
	if (!m_stopDecode)
	{
		return;
	}

	// 设置标志
	m_stopDecode = true;
	while (m_stopDecode)
	{
		std::this_thread::sleep_for(m_interval);
	}
}