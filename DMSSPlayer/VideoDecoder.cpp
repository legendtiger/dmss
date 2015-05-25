#include "VideoDecoder.h"

using namespace dmss::ffmpeg;

VideoDecoder::VideoDecoder(DecodeInfo info, AVPixelFormat fmt)
:IDecoder(info), m_fmt(fmt)
{
	// 找到对应解码器
	m_pVCodec = avcodec_find_decoder(m_info.pVCodecCtx->codec_id);
	if (m_pVCodec == NULL)
	{
		throw "找不到对应解码器！";
	}

	// 打开解码器
	if (avcodec_open2(m_info.pVCodecCtx, m_pVCodec, NULL) < 0)
	{
		throw "打开解码器失败！";
	}

	// 分配解码帧
	this->m_pFrame = av_frame_alloc();
	this->m_pFrameRGB = av_frame_alloc();
	if (m_pFrame == NULL || m_pFrameRGB == NULL)
	{
		Destroy();
		throw "av_frame_alloc调用失败！";
	}

	// 初始化填充目标帧
	if (avpicture_alloc((AVPicture *)m_pFrameRGB, m_fmt, m_info.pVCodecCtx->width, m_info.pVCodecCtx->height) < 0)
	{
		Destroy();
		throw "avpicture_alloc调用失败！";
	}

	// 创建图形转换对象
	m_pSwsCtx = sws_getContext(m_info.pVCodecCtx->width, m_info.pVCodecCtx->height,
		m_info.pVCodecCtx->pix_fmt, m_info.pVCodecCtx->width, m_info.pVCodecCtx->height, m_fmt, SWS_BICUBIC, NULL, NULL, NULL);
	if (m_pSwsCtx == NULL)
	{
		Destroy();
		throw "sws_getContext调用失败！";
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
	// 不是初始化解码信息对应视频包
	if (packet.stream_index != m_info.videoIndex)
	{
		return false;
	}

	// 解码
	avcodec_decode_video2(m_info.pVCodecCtx, m_pFrame, &frameFinished, &packet);

	// 得到完整视频帧
	if (frameFinished != 0)
	{
		// 转换为指定格式
		
		sws_scale(m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, m_pFrame->height, m_pFrameRGB->data, m_pFrameRGB->linesize);

		m_length = m_pFrameRGB->linesize[0];
		m_pData = m_pFrameRGB->data[0]; 
		result = true;

		// 通知监听对象
		Update(m_pData, m_length);
	}

	return result;
}