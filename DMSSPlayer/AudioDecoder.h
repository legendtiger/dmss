#ifndef __DMSS_AUDIODECODER_H
#define __DMSS_AUDIODECODER_H

#include "IDecoder.h"

namespace dmss
{
	namespace ffmpeg
	{
		struct AudioLayout
		{
			// �����ʽ
			AVSampleFormat format = AV_SAMPLE_FMT_S16;

			// ͨ������
			int channel_layout;

			// ����ͨ����
			int channels;

			// ������
			int sample_rate;

			// true����ʾ����Դ����channel_layout��channels��sample_rate
			// false��ֱ��ʹ��channel_layout��channels��sample_rate
			bool adapting = true;
		};

		class AudioDecoder : public IDecoder
		{
		public:
			AudioDecoder(DecodeInfo info, AudioLayout dstLayout);
			~AudioDecoder();

		private:
			// �ҵ���Ӧ������
			AVCodec *m_pACodec = NULL;

			// ת�������ƵĿ��
			AVFrame* m_pAudioFrame = NULL;

			// Ŀ�������ʽ
			AudioLayout m_dstLayout;

			// ��Ƶת������
			struct SwrContext *m_pASwrCtx = NULL;

			// ת�����ݻ�����
			uint8_t **m_pAudioBuffer = NULL;

			// ��������С
			int m_audioSize;

			void * m_pData;
			int m_length;

		public:
			// ���
			bool Unpacking(AVPacket &package);

			// ��ȡ��������������һ��������ã���Ϊ������ʾ����
			// *data �������ݣ� �������ݳ���
			int GetResult(void **data);

		private:
			void Destroy();

		public:
			// ����һ������Դ���������AudioLayout����
			static AudioLayout CreateAudioLayout(AVSampleFormat format);

			// ����һ��ָ��channel_layout��channels��sample_rate��AudioLayout����
			static AudioLayout CreateAudioLayout(AVSampleFormat format, int channel_layout, int channels, int sample_rate);
		};
	}
}

#endif