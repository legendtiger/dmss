#ifndef __DMSS_AUDIODECODER_H
#define __DMSS_AUDIODECODER_H

#include "IDecoder.h"

namespace dmss
{
	namespace ffmpeg
	{
		struct AudioLayout
		{
			// 编码格式
			AVSampleFormat format = AV_SAMPLE_FMT_S16;

			// 通道布局
			int channel_layout;

			// 采用通道数
			int channels;

			// 采样率
			int sample_rate;

			// true，表示根据源适配channel_layout、channels和sample_rate
			// false，直接使用channel_layout、channels和sample_rate
			bool adapting = true;
		};

		class AudioDecoder : public IDecoder
		{
		public:
			AudioDecoder(DecodeInfo info, AudioLayout dstLayout);
			~AudioDecoder();

		private:
			// 找到对应解码器
			AVCodec *m_pACodec = NULL;

			// 转换后的音频目标
			AVFrame* m_pAudioFrame = NULL;

			// 目标采样格式
			AudioLayout m_dstLayout;

			// 音频转换对象
			struct SwrContext *m_pASwrCtx = NULL;

			// 转码数据缓冲区
			uint8_t **m_pAudioBuffer = NULL;

			// 缓冲区大小
			int m_audioSize;

			void * m_pData;
			int m_length;

		public:
			// 解包
			bool Unpacking(AVPacket &package);

			// 获取解码结果，解析完一个包后调用，作为更新显示数据
			// *data 保存数据， 返回数据长度
			int GetResult(void **data);

		private:
			void Destroy();

		public:
			// 创建一个根据源适配适配的AudioLayout对象
			static AudioLayout CreateAudioLayout(AVSampleFormat format);

			// 创建一个指定channel_layout、channels和sample_rate的AudioLayout对象
			static AudioLayout CreateAudioLayout(AVSampleFormat format, int channel_layout, int channels, int sample_rate);
		};
	}
}

#endif