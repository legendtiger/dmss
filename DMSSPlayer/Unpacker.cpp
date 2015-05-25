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

// ��ʼ��ý��
bool Unpacker::Init(std::string url)
{
	ResetDecodeInfo();

	return GetFormatContext(url);
}

// ��ָ��λ�ÿ�ʼ����, startָ��λ�ã���λΪ��			
void Unpacker::Unpacking(int start)
{
	// �˳������߳�
	ExitThread();

	// ���������߳�
	m_startTime = start;

	std::thread thread(DecodeThread, this);
	thread.detach();
}

void Unpacker::ReadPacket()
{
	// ��ʼλ��
	int startBase = m_startTime * AV_TIME_BASE;		

	// �������λ��
	av_seek_frame(m_decodeInfo.pFormatCtx, -1, startBase, AVSEEK_FLAG_FRAME);

	DecodeInfo &info = m_decodeInfo;
	AVRational time_base_q = { 1, AV_TIME_BASE };

	// ����timestamp����
	auto Timestamp = [&](AVPacket packet)->int64_t
	{ 
		return av_rescale_q(packet.pts, info.pFormatCtx->streams[packet.stream_index]->time_base, time_base_q);
	};

	auto Duration = [&](AVPacket packet)->int64_t
	{
		return av_rescale_q(packet.duration, info.pFormatCtx->streams[packet.stream_index]->time_base, time_base_q);
	};

	// ��ʼ�����ݰ�
	AVPacket packet;	
	av_init_packet(&packet);

	// ���
	PacketItem *item = NULL;
	while (av_read_frame(m_decodeInfo.pFormatCtx, &packet) >= 0)
	{
		//fprintf(stdout, "pts = %d�������ϣ�", item->packet.pts);
		// ֹͣ�����ݰ�
		if (m_stopDecode)
		{
			break;
		}

		if (!(packet.stream_index == info.audioIndex || packet.stream_index == info.videoIndex))
		{
			av_free_packet(&packet);
			continue;
		}

		// ����timestamp
		item = new PacketItem();
		item->packet = packet;
		item->timestamp = Timestamp(item->packet);
		item->duration = Duration(item->packet);

		//fprintf(stdout, "timestamp=%u, stream_index = %d\n", item->timestamp, packet.stream_index);
		
		// ����������ݰ����ݵ����Ŷ���
		Dispatch(item);

		// �ͷ�CPUռ��
		std::this_thread::sleep_for(m_interval);
	}

	// �������һ���հ�
	item = new PacketItem();
	item->packet.stream_index = info.audioIndex;
	item->isLast = true;
	Dispatch(item);

	item = new PacketItem();
	item->packet.stream_index = info.videoIndex;
	item->isLast = true;
	Dispatch(item);

	m_stopDecode = false;
	fprintf(stdout, "Unpacker�����ϣ�");
}

bool Unpacker::GetFormatContext(std::string url)
{
	m_decodeInfo.url = url;

	// ��ý���ļ�
	if (avformat_open_input(&m_decodeInfo.pFormatCtx, url.c_str(), NULL, NULL) != 0)
	{
		return false;
	}

	// ��ȡ����Ϣ
	if (avformat_find_stream_info(m_decodeInfo.pFormatCtx, NULL) < 0)
	{
		return false;
	}	

	// ý�峤��
	m_decodeInfo.duration = m_decodeInfo.pFormatCtx->duration / AV_TIME_BASE;

	// ��Ƶ��
	m_decodeInfo.videoIndex = FindStream(AVMEDIA_TYPE_VIDEO);
	if (m_decodeInfo.videoIndex != -1)
	{
		m_decodeInfo.pVCodecCtx = m_decodeInfo.pFormatCtx->streams[m_decodeInfo.videoIndex]->codec;		
		m_decodeInfo.mediaType = MediaType::ONLEVIDEO;
	}

	// ��Ƶ��
	m_decodeInfo.audioIndex = FindStream(AVMEDIA_TYPE_AUDIO);
	if (m_decodeInfo.audioIndex != -1)
	{
		m_decodeInfo.pACodecCtx = m_decodeInfo.pFormatCtx->streams[m_decodeInfo.audioIndex]->codec;
		m_decodeInfo.mediaType = MediaType(0 | m_decodeInfo.mediaType | MediaType::ONLEAUDIO);
	}	

	// ��ӡý����Ϣ
	av_dump_format(m_decodeInfo.pFormatCtx, 0, url.c_str(), 0);
	return true;
}

// ��ȡ��
int Unpacker::FindStream(AVMediaType type)
{
	// �ҵ���Ƶ����Ƶ��
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
	// �˳��߳�
	ExitThread();

	// �ͷ���Դ
	ResetDecodeInfo();
}

void Unpacker::ExitThread()
{
	// �߳�û������
	if (!m_stopDecode)
	{
		return;
	}

	// ���ñ�־
	m_stopDecode = true;
	while (m_stopDecode)
	{
		std::this_thread::sleep_for(m_interval);
	}
}