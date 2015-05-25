#ifndef __DMSS_IUNPACKING_H
#define __DMSS_IUNPACKING_H

#include "Object.h"
#include <string>
#include "ImportFFMpeg.h"

namespace dmss
{
	namespace ffmpeg
	{
		class IPlayer;
		struct PacketItem;
		enum MediaType
		{
			UNKNOWN = 0,
			ONLEVIDEO,
			ONLEAUDIO,
			VIDEOANDAUDIO = ONLEVIDEO | ONLEAUDIO
		};

		struct DecodeInfo
		{
			// ý���ַ
			std::string url = "";

			// ��¼ý�����
			AVFormatContext *pFormatCtx = NULL;

			// ��¼����Ƶ���������ĺͽ�����
			AVCodecContext *pVCodecCtx = NULL;			
			int videoIndex = -1;
			AVCodecContext *pACodecCtx = NULL;
			int audioIndex = -1;

			// ��¼ý������
			MediaType mediaType = UNKNOWN;

			// ��¼ý��ʱ��,��λ��
			int64_t duration = 0;

			// ����
			void Reset();

			// �õ���ʽ�����ʱ��(��ʽ:hh:mm:ss)
			std::string Duration();
		};

		class IUnpacking : public Object
		{
		public:
			IUnpacking(IPlayer* player);
			~IUnpacking();

		private:
			// ����������
			IPlayer* m_player = NULL;

		protected:
			// ý�������Ϣ
			DecodeInfo m_decodeInfo;

		public:
			// ��ʼ��ý��
			virtual bool Init(std::string url) = 0;

			// ��ָ��λ�ÿ�ʼ���, startָ��λ�ã���λΪ��			
			virtual void Unpacking(int start = 0) = 0;

			// ý�������Ϣ
			DecodeInfo  GetDecodeInfo();

		protected:
			// �̳е������ڽ���������ñ������ַ�ý���
			void Dispatch(PacketItem *packet);
			
			// ��ʼ��ý�������Ϣ
			void ResetDecodeInfo();
		};
	}
}

#endif