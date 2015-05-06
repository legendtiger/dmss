#ifndef __DMSS_FFAVDECODER_H
#define __DMSS_FFAVDECODER_H
#include "FFCommon.h"
#include "SDLVideoWnd.h"
#include <thread>

class FFAVDecoder
{
private:
	static const int DELAYTIME = 1;
	static const int MAX_AUDIO_FRAME_SIZE = 192000;

private:
	// ��¼ý�����
	AVFormatContext *m_pFormatCtx = NULL;

	// ��¼ת��Ŀ���ʽ
	AVPixelFormat m_fmt;

	// ��¼����Ƶͨ��
	int m_videoStream = -1;
	int m_audioStream = -1;

	// ��¼����Ƶ���뷽ʽ
	AVCodecContext *m_pVCodecCtx = NULL;
	AVCodecContext *m_pACodecCtx = NULL;

	// �ҵ���Ӧ������
	AVCodec *m_pVCodec = NULL;
	AVCodec *m_pACodec = NULL;

	// ԭʼ��Ƶ֡
	AVFrame* m_pFrame = NULL;

	// ת�����Ŀ����Ƶ֡
	AVFrame* m_pFrameRGB = NULL;
	// ת�������ƵĿ��
	AVFrame* m_pAudioFrame = NULL;
	uint8_t *m_pAudioBuffer = NULL;
	int m_audioSize;

	// ������Ҫ�Ļ�����
	uint8_t *m_buffer = NULL;

	// ͼ��ת������
	struct SwsContext *m_pSwsCtx = NULL;

	// ��Ƶת������
	struct SwrContext *m_pASwrCtx = NULL;

	SDLVideoWnd & m_textuer;

	std::thread *m_pThreadDecoder;

public:
	FFAVDecoder(SDLVideoWnd &texture, AVPixelFormat fmt = AV_PIX_FMT_YUV420P);
	~FFAVDecoder();

public:
	// ������
	int Play(std::string url);

private:
	void decoding();

	// ��ȡ��ʽ��Ϣ
	bool GetFormatContext(std::string url);

	// ��ȡ��
	int FindStream(AVMediaType type);

private:
	// ����ý���ļ�
	static int Run(FFAVDecoder* decoder);

	// ȡϵͳ��ǰʱ�䣬����
	static uint64_t CurrentTime();

	// ������Ƶ����
	bool DecodingVedio();
};

#endif