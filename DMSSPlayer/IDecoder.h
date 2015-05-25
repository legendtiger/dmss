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
			// �����Ϣ
			DecodeInfo m_info;

		public:
			IDecoder(DecodeInfo info);
			virtual ~IDecoder();

		public:
			// ���
			virtual bool Unpacking(AVPacket &packet) = 0;

			// ��ȡ��������������һ��������ã���Ϊ������ʾ����
			// *data �������ݣ� �������ݳ���
			virtual int GetResult(void **data) = 0;
		};
	}
}
#endif