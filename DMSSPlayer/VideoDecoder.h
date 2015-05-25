#ifndef __DMSS_VIDEODECODER_H
#define __DMSS_VIDEODECODER_H

#include "Object.h"
#include "ImportFFMpeg.h"
#include "IDecoder.h"

namespace dmss
{
	namespace ffmpeg
	{
		// ��Ƶ���빤��
		class VideoDecoder : public IDecoder
		{
		private:
			// Ŀ��ͼ���ʽ
			AVPixelFormat m_fmt;

			// �ҵ���Ӧ������
			AVCodec* m_pVCodec = NULL;

			// ԭʼ��Ƶ֡
			AVFrame* m_pFrame = NULL;

			// ת�����Ŀ����Ƶ֡
			AVFrame* m_pFrameRGB = NULL;

			// ͼ��ת������
			struct SwsContext *m_pSwsCtx = NULL;

			// ��������ݳ���
			int m_length = 0;

			// ��������ݻ������׵�ַ
			void *m_pData = NULL;

		public:
			VideoDecoder(DecodeInfo info, AVPixelFormat fmt = AV_PIX_FMT_YUV420P);
			~VideoDecoder();

		public:
			// ���
			bool Unpacking(AVPacket &package);

			// ��ȡ��������������һ��������ã���Ϊ������ʾ����
			// *data �������ݣ� �������ݳ���
			int GetResult(void **data);

		private:
			void Destroy();
		};
	}
}

#endif