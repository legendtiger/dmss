#include "AudioDecoder.h"

using namespace dmss::ffmpeg;

AudioDecoder::AudioDecoder(DecodeInfo info, AudioLayout dstLayout)
:IDecoder(info), m_dstLayout(dstLayout)
{
	// �ҵ���Ӧ������
	m_pACodec = avcodec_find_decoder(m_info.pACodecCtx->codec_id);
	if (m_pACodec == NULL)
	{
		throw "avcodec_find_decoderû�ж�Ӧ�Ľ�������";
	}

	// �򿪽�����
	if (avcodec_open2(m_info.pACodecCtx, m_pACodec, NULL) < 0)
	{
		throw "����avcodec_open2�����򿪽�����ʧ�ܣ�";
	}

	// �������֡
	this->m_pAudioFrame = av_frame_alloc();
	if (m_pAudioFrame == NULL)
	{
		throw "����av_frame_alloc()ʧ�ܣ�";
	}
	
	// Ŀ�������ʽ
	if (m_dstLayout.adapting)
	{
		m_dstLayout.channel_layout = m_info.pACodecCtx->channel_layout>0 ? m_info.pACodecCtx->channel_layout : av_get_default_channel_layout(m_info.pACodecCtx->channels);
		m_dstLayout.channels = m_info.pACodecCtx->channels;
		m_dstLayout.sample_rate = m_info.pACodecCtx->sample_rate;
	}

	// ����ת��buffer  
	av_samples_alloc_array_and_samples(&m_pAudioBuffer, &m_audioSize, 
		m_dstLayout.channels, m_dstLayout.sample_rate, m_dstLayout.format, 0);
	//fprintf(stdout, "channels = %d, sample_rate = %d, format = %d", m_dstLayout.channels, m_dstLayout.sample_rate, m_dstLayout.format);

	// ����ת�����
	//int channel_layout = m_info.pACodecCtx->channel_layout>0 ? m_info.pACodecCtx->channel_layout : av_get_default_channel_layout(m_info.pACodecCtx->channels);
	m_pASwrCtx = swr_alloc_set_opts(m_pASwrCtx, m_dstLayout.channel_layout, m_dstLayout.format, m_dstLayout.sample_rate,
		m_dstLayout.channel_layout, m_info.pACodecCtx->sample_fmt, m_info.pACodecCtx->sample_rate, 0, NULL);
	if (m_pASwrCtx == NULL)
	{
		Destroy();
		throw "����swr_alloc_set_opts()ʧ�ܣ�";
	}
	swr_init(m_pASwrCtx);
}


AudioDecoder::~AudioDecoder()
{
	Destroy();
}

// ���
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

			// ֪ͨ��������
			Update(m_pData, m_length);
		}
	}

	return true;
}

// ��ȡ��������������һ��������ã���Ϊ������ʾ����
// *data �������ݣ� �������ݳ���
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

// ����һ������Դ���������AudioLayout����
AudioLayout AudioDecoder::CreateAudioLayout(AVSampleFormat format)
{
	AudioLayout layout;
	layout.format = format;
	return layout;
}

// ����һ��ָ��channel_layout��channels��sample_rate��AudioLayout����
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