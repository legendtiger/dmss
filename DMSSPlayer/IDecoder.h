#ifndef __DMSS_IDECODER_H
#define __DMSS_IDECODER_H

#include "IUnpacking.h"

namespace dmss
{
	namespace ffmpeg
	{
		class IDecoder : public Observable
		{
		protected:
			// 解包信息
			DecodeInfo m_info;

		public:
			IDecoder(DecodeInfo info);
			virtual ~IDecoder();

		public:
			// 解包
			virtual bool Unpacking(AVPacket &packet) = 0;

			// 获取解码结果，解析完一个包后调用，作为更新显示数据
			// *data 保存数据， 返回数据长度
			virtual int GetResult(void **data) = 0;
		};
	}
}
#endif