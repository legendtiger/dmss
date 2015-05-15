#ifndef __DMSS_IDECODER_H
#define __DMSS_IDECODER_H

#include "Object.h"
#include <string>
#include "ImportFFMpeg.h"

namespace dmss
{
	namespace ffmpeg
	{
		class IPlayer;
		enum MediaType
		{
			UNKNOWN = 0,
			ONLEVIDEO,
			ONLEAUDIO,
			VIDEOANDAUDIO = ONLEVIDEO | ONLEAUDIO
		};

		struct DecodeInfo
		{
			// 媒体地址
			std::string url = "";

			// 记录媒体参数
			AVFormatContext *pFormatCtx = NULL;

			// 记录音视频编码上下文和解码器
			AVCodecContext *pVCodecCtx = NULL;			
			int videoIndex = -1;
			AVCodecContext *pACodecCtx = NULL;
			int audioIndex = -1;

			// 记录媒体类型
			MediaType mediaType = UNKNOWN;

			// 记录媒体时长,单位秒
			int64_t duration = 0;

			// 重设
			void Reset();

			// 得到格式化后的时长(格式:hh:mm:ss)
			std::string Duration();
		};

		class IDecoder : public Object
		{
		public:
			IDecoder(IPlayer* player);
			~IDecoder();

		private:
			// 播放器对象
			IPlayer* m_player = NULL;

		protected:
			// 媒体解码信息
			DecodeInfo m_decodeInfo;

		public:
			// 初始化媒体
			virtual bool Init(std::string url) = 0;

			// 从指定位置开始解包, start指定位置，单位为秒			
			virtual void Decoding(int start = 0) = 0;

			// 媒体解码信息
			DecodeInfo & DecodeInfo();

		protected:
			// 继承的子类在解码后必须调用本方法分发媒体包
			void Dispatch(AVPacket &packet);
			
			// 初始化媒体解码信息
			void ResetDecodeInfo();
		};
	}
}

#endif