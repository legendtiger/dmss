#ifndef __DMSS_VIDEODECODER_H
#define __DMSS_VIDEODECODER_H

#include "Object.h"
#include "ImportFFMpeg.h"
#include "IDecoder.h"

namespace dmss
{
	namespace ffmpeg
	{
		// 视频解码工具
		class VideoDecoder : public IDecoder
		{
		private:
			// 目标图像格式
			AVPixelFormat m_fmt;

			// 找到对应解码器
			AVCodec* m_pVCodec = NULL;

			// 原始视频帧
			AVFrame* m_pFrame = NULL;

			// 转换后的目标视频帧
			AVFrame* m_pFrameRGB = NULL;

			// 图形转换对象
			struct SwsContext *m_pSwsCtx = NULL;

			// 解码后数据长度
			int m_length = 0;

			// 解码后数据缓冲区首地址
			void *m_pData = NULL;

		public:
			VideoDecoder(DecodeInfo info, AVPixelFormat fmt = AV_PIX_FMT_YUV420P);
			~VideoDecoder();

		public:
			// 解包
			bool Unpacking(AVPacket &package);

			// 获取解码结果，解析完一个包后调用，作为更新显示数据
			// *data 保存数据， 返回数据长度
			int GetResult(void **data);

		private:
			void Destroy();
		};
	}
}

#endif