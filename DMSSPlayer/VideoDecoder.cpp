#include "VideoDecoder.h"

using namespace dmss::ffmpeg;

VideoDecoder::VideoDecoder(DecodeInfo info, AVPixelFormat fmt)
:IDecoder(info), m_fmt(fmt)
{
	// �ҵ���Ӧ������
	m_pVCodec = avcodec_find_decoder(m_info.pVCodecCtx->codec_id);
	if (m_pVCodec == NULL)
	{
		throw "�Ҳ�����Ӧ��������";
	}

	// �򿪽�����
	if (avcodec_open2(m_info.pVCodecCtx, m_pVCodec, NULL) < 0)
	{
		throw "�򿪽�����ʧ�ܣ�";
	}

	// �������֡
	this->m_pFrame = av_frame_alloc();
	this->m_pFrameRGB = av_frame_alloc();
	if (m_pFrame == NULL || m_pFrameRGB == NULL)
	{
		Destroy();
		throw "av_frame_alloc����ʧ�ܣ�";
	}

	// ��ʼ�����Ŀ��֡
	if (avpicture_alloc((AVPicture *)m_pFrameRGB, m_fmt, m_info.pVCodecCtx->width, m_info.pVCodecCtx->height) < 0)
	{
		Destroy();
		throw "avpicture_alloc����ʧ�ܣ�";
	}

	// ����ͼ��ת������
	m_pSwsCtx = sws_getContext(m_info.pVCodecCtx->width, m_info.pVCodecCtx->height,
		m_info.pVCodecCtx->pix_fmt, m_info.pVCodecCtx->width, m_info.pVCodecCtx->height, m_fmt, SWS_BICUBIC, NULL, NULL, NULL);
	if (m_pSwsCtx == NULL)
	{
		Destroy();
		throw "sws_getContext����ʧ�ܣ�";
	}
}

void VideoDecoder::Destroy()
{
	av_frame_free(&m_pFrame);
	av_frame_free(&m_pFrameRGB);
	sws_freeContext(m_pSwsCtx);
}


VideoDecoder::~VideoDecoder()
{
	Destroy();
}

int VideoDecoder::GetResult(void **data)
{
	*data = m_pData;
	return m_length;
}

bool VideoDecoder::Unpacking(AVPacket &packet)
{
	int frameFinished = -1;
	bool result = false;
	// ���ǳ�ʼ��������Ϣ��Ӧ��Ƶ��
	if (packet.stream_index != m_info.videoIndex)
	{
		return false;
	}

	// ����
	avcodec_decode_video2(m_info.pVCodecCtx, m_pFrame, &frameFinished, &packet);

	// �õ�������Ƶ֡
	if (frameFinished != 0)
	{
		// ת��Ϊָ����ʽ
		
		sws_scale(m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, m_pFrame->height, m_pFrameRGB->data, m_pFrameRGB->linesize);

		m_length = m_pFrameRGB->linesize[0];
		m_pData = m_pFrameRGB->data[0]; 
		result = true;

		// ֪ͨ��������
		Update(m_pData, m_length);
	}

	return result;
}