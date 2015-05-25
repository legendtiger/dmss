#include "AudioDecoder.h"

using namespace dmss::ffmpeg;

AudioDecoder::AudioDecoder(DecodeInfo info, AudioLayout dstLayout)
:IDecoder(info), m_dstLayout(dstLayout)
{
	// 找到对应解码器
	m_pACodec = avcodec_find_decoder(m_info.pACodecCtx->codec_id);
	if (m_pACodec == NULL)
	{
		throw "avcodec_find_decoder没有对应的解码器！";
	}

	// 打开解码器
	if (avcodec_open2(m_info.pACodecCtx, m_pACodec, NULL) < 0)
	{
		throw "调用avcodec_open2方法打开解码器失败！";
	}

	// 分配解码帧
	this->m_pAudioFrame = av_frame_alloc();
	if (m_pAudioFrame == NULL)
	{
		throw "调用av_frame_alloc()失败！";
	}
	
	// 目标采样格式
	if (m_dstLayout.adapting)
	{
		m_dstLayout.channel_layout = m_info.pACodecCtx->channel_layout>0 ? m_info.pACodecCtx->channel_layout : av_get_default_channel_layout(m_info.pACodecCtx->channels);
		m_dstLayout.channels = m_info.pACodecCtx->channels;
		m_dstLayout.sample_rate = m_info.pACodecCtx->sample_rate;
	}

	// 分配转码buffer  
	av_samples_alloc_array_and_samples(&m_pAudioBuffer, &m_audioSize, 
		m_dstLayout.channels, m_dstLayout.sample_rate, m_dstLayout.format, 0);
	//fprintf(stdout, "channels = %d, sample_rate = %d, format = %d", m_dstLayout.channels, m_dstLayout.sample_rate, m_dstLayout.format);

	// 创建转码对象
	//int channel_layout = m_info.pACodecCtx->channel_layout>0 ? m_info.pACodecCtx->channel_layout : av_get_default_channel_layout(m_info.pACodecCtx->channels);
	m_pASwrCtx = swr_alloc_set_opts(m_pASwrCtx, m_dstLayout.channel_layout, m_dstLayout.format, m_dstLayout.sample_rate,
		m_dstLayout.channel_layout, m_info.pACodecCtx->sample_fmt, m_info.pACodecCtx->sample_rate, 0, NULL);
	if (m_pASwrCtx == NULL)
	{
		Destroy();
		throw "调用swr_alloc_set_opts()失败！";
	}
	swr_init(m_pASwrCtx);
}


AudioDecoder::~AudioDecoder()
{
	Destroy();
}

// 解包
bool AudioDecoder::Unpacking(AVPacket &packet)
{
	if (packet.stream_index != m_info.audioIndex)
	{
		return false;
	}

	int frameFinished = 0;
	if (avcodec_decode_audio4(m_info.pACodecCtx, m_pAudioFrame, &frameFinished, &packet) < 0)
	{
		return false;
	}

	if (frameFinished >0)
	{
		int numSamplesOut = swr_convert(m_pASwrCtx, m_pAudioBuffer, m_audioSize, (const uint8_t**)m_pAudioFrame->data, m_pAudioFrame->nb_samples);
		if (numSamplesOut > 0)
		{
			m_length = numSamplesOut * m_dstLayout.channels * av_get_bytes_per_sample(m_dstLayout.format);
			m_pData = m_pAudioBuffer[0];

			// 通知监听对象
			Update(m_pData, m_length);
		}
	}

	return true;
}

// 获取解码结果，解析完一个包后调用，作为更新显示数据
// *data 保存数据， 返回数据长度
int AudioDecoder::GetResult(void **data)
{
	*data = m_pData;
	return m_length;
}

void AudioDecoder::Destroy()
{
	av_frame_free(&m_pAudioFrame);
	if (m_pAudioBuffer)
	{
		av_freep(&m_pAudioBuffer[0]);
	}
	av_freep(&m_pAudioBuffer);

	swr_free(&m_pASwrCtx);
}

// 创建一个根据源适配适配的AudioLayout对象
AudioLayout AudioDecoder::CreateAudioLayout(AVSampleFormat format)
{
	AudioLayout layout;
	layout.format = format;
	return layout;
}

// 创建一个指定channel_layout、channels和sample_rate的AudioLayout对象
AudioLayout AudioDecoder::CreateAudioLayout(AVSampleFormat format, int channel_layout, int channels, int sample_rate)
{
	AudioLayout layout;
	layout.format = format;
	layout.channel_layout = channel_layout;
	layout.channels = channels;
	layout.sample_rate = sample_rate;
	layout.adapting = false;
	return layout;
}