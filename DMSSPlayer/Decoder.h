#ifndef __DMSS_DECODER_H
#define __DMSS_DECODER_H
#include "IDecoder.h"
#include <thread>

namespace dmss
{
	namespace ffmpeg
	{
		class Decoder : public IDecoder
		{
			// 默认解压休眠间隔，单位毫秒
			static const int SLEEPTIME = 10;

		public:
			Decoder(IPlayer* player, int interval = SLEEPTIME);
			~Decoder();

		private:
			// 结束线程标志
			bool m_stopDecode = false;

			// 休眠间隔
			std::chrono::milliseconds m_interval;

			// 开始时间
			int m_startTime = 0;
			
		public:
			// 初始化媒体
			bool Init(std::string url);

			// 从指定位置开始解码, start指定位置，单位为秒			
			void Decoding(int start = 0);

			void Destroy();

		private:
			// 获取格式信息
			bool GetFormatContext(std::string url);

			// 获取流
			int FindStream(AVMediaType type);

			// 从媒体中读取包
			void ReadPacket();

			void ExitThread();

			// 解包线程调用方法
			static void DecodeThread(Decoder* decoder);
		};
	}
}

#endif