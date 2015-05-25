#include "Unpacker.h"
#include "IPlayer.h"

using namespace dmss::ffmpeg;

Unpacker::Unpacker(IPlayer* player, int interval)
:IUnpacking(player), m_interval(interval)
{
	avcodec_register_all();
	avfilter_register_all();
	av_register_all();
	avformat_network_init();
}


Unpacker::~Unpacker()
{
}

// 初始化媒体
bool Unpacker::Init(std::string url)
{
	ResetDecodeInfo();

	return GetFormatContext(url);
}

// 从指定位置开始解码, start指定位置，单位为秒			
void Unpacker::Unpacking(int start)
{
	// 退出已有线程
	ExitThread();

	// 开启解码线程
	m_startTime = start;

	std::thread thread(DecodeThread, this);
	thread.detach();
}

void Unpacker::ReadPacket()
{
	// 起始位置
	int startBase = m_startTime * AV_TIME_BASE;		

	// 跳到解包位置
	av_seek_frame(m_decodeInfo.pFormatCtx, -1, startBase, AVSEEK_FLAG_FRAME);

	DecodeInfo &info = m_decodeInfo;
	AVRational time_base_q = { 1, AV_TIME_BASE };

	// 计算timestamp方法
	auto Timestamp = [&](AVPacket packet)->int64_t
	{ 
		return av_rescale_q(packet.pts, info.pFormatCtx->streams[packet.stream_index]->time_base, time_base_q);
	};

	auto Duration = [&](AVPacket packet)->int64_t
	{
		return av_rescale_q(packet.duration, info.pFormatCtx->streams[packet.stream_index]->time_base, time_base_q);
	};

	// 初始化数据包
	AVPacket packet;	
	av_init_packet(&packet);

	// 解包
	PacketItem *item = NULL;
	while (av_read_frame(m_decodeInfo.pFormatCtx, &packet) >= 0)
	{
		//fprintf(stdout, "pts = %d，解包完毕！", item->packet.pts);
		// 停止读数据包
		if (m_stopDecode)
		{
			break;
		}

		if (!(packet.stream_index == info.audioIndex || packet.stream_index == info.videoIndex))
		{
			av_free_packet(&packet);
			continue;
		}

		// 计算timestamp
		item = new PacketItem();
		item->packet = packet;
		item->timestamp = Timestamp(item->packet);
		item->duration = Duration(item->packet);

		//fprintf(stdout, "timestamp=%u, stream_index = %d\n", item->timestamp, packet.stream_index);
		
		// 将解出的数据包传递到播放对象
		Dispatch(item);

		// 释放CPU占用
		std::this_thread::sleep_for(m_interval);
	}

	// 放置最后一个空包
	item = new PacketItem();
	item->packet.stream_index = info.audioIndex;
	item->isLast = true;
	Dispatch(item);

	item = new PacketItem();
	item->packet.stream_index = info.videoIndex;
	item->isLast = true;
	Dispatch(item);

	m_stopDecode = false;
	fprintf(stdout, "Unpacker解包完毕！");
}

bool Unpacker::GetFormatContext(std::string url)
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
	if (m_decodeInfo.audioIndex != -1)
	{
		m_decodeInfo.pACodecCtx = m_decodeInfo.pFormatCtx->streams[m_decodeInfo.audioIndex]->codec;
		m_decodeInfo.mediaType = MediaType(0 | m_decodeInfo.mediaType | MediaType::ONLEAUDIO);
	}	

	// 打印媒体信息
	av_dump_format(m_decodeInfo.pFormatCtx, 0, url.c_str(), 0);
	return true;
}

// 获取流
int Unpacker::FindStream(AVMediaType type)
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

void Unpacker::DecodeThread(Unpacker* unpacker)
{
	unpacker->ReadPacket();
}

void Unpacker::Destroy()
{
	// 退出线程
	ExitThread();

	// 释放资源
	ResetDecodeInfo();
}

void Unpacker::ExitThread()
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