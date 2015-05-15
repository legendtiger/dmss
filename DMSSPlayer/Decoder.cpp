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

// ��ʼ��ý��
bool Decoder::Init(std::string url)
{
	ResetDecodeInfo();

	return GetFormatContext(url);
}

// ��ָ��λ�ÿ�ʼ����, startָ��λ�ã���λΪ��			
void Decoder::Decoding(int start)
{
	// �˳������߳�
	ExitThread();

	// ���������߳�
	m_startTime = start;
	std::thread thread(DecodeThread, this);
	thread.detach();
}

void Decoder::ReadPacket()
{
	// ��ʼλ��
	int startBase = m_startTime * AV_TIME_BASE;	

	// ��ʼ�����ݰ�
	AVPacket packet;
	av_init_packet(&packet);

	// �������λ��
	av_seek_frame(m_decodeInfo.pFormatCtx, -1, startBase, AVSEEK_FLAG_FRAME);

	// ���
	while (av_read_frame(m_decodeInfo.pFormatCtx, &packet) >= 0)
	{
		// ֹͣ�����ݰ�
		if (m_stopDecode)
		{
			break;
		}

		// ����������ݰ����ݵ����Ŷ���
		Dispatch(packet);

		std::this_thread::sleep_for(m_interval);
	}

	m_stopDecode = false;
}

bool Decoder::GetFormatContext(std::string url)
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
	if (m_decodeInfo.videoIndex != -1)
	{
		m_decodeInfo.pACodecCtx = m_decodeInfo.pFormatCtx->streams[m_decodeInfo.audioIndex]->codec;
		m_decodeInfo.mediaType = MediaType(0 | m_decodeInfo.mediaType | MediaType::ONLEAUDIO);
	}	

	// ��ӡý����Ϣ
	av_dump_format(m_decodeInfo.pFormatCtx, 0, url.c_str(), 0);
	return true;
}

// ��ȡ��
int Decoder::FindStream(AVMediaType type)
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

void Decoder::DecodeThread(Decoder* decoder)
{
	decoder->ReadPacket();
}

void Decoder::Destroy()
{
	// �˳��߳�
	ExitThread();

	// �ͷ���Դ
	ResetDecodeInfo();
}

void Decoder::ExitThread()
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