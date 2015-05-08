#ifndef __DMSS_FFAVDECODER_H
#define __DMSS_FFAVDECODER_H
#include "FFCommon.h"
#include "SDLVideoWnd.h"
#include <thread>

enum FFDecoderStatus
{
	UNRIPE = -1,
	PREPARED,
	PLAYING,
	PUASE,
	STOP
};

class FFAVDecoder
{
private:
	static const int DELAYTIME = 10;
	static const int MAX_AUDIO_FRAME_SIZE = 192000;

private:
	// 记录媒体参数
	AVFormatContext *m_pFormatCtx = NULL;

	// 记录转换目标格式
	AVPixelFormat m_fmt;

	// 记录音视频通道
	int m_videoStream = -1;
	int m_audioStream = -1;

	// 记录音视频编码方式
	AVCodecContext *m_pVCodecCtx = NULL;
	AVCodecContext *m_pACodecCtx = NULL;

	// 找到对应解码器
	AVCodec *m_pVCodec = NULL;
	AVCodec *m_pACodec = NULL;

	// 原始视频帧
	AVFrame* m_pFrame = NULL;

	// 转换后的目标视频帧
	AVFrame* m_pFrameRGB = NULL;
	// 转换后的音频目标
	AVFrame* m_pAudioFrame = NULL;
	uint8_t **m_pAudioBuffer = NULL;
	int m_audioSize;

	// 解码需要的缓冲区
	uint8_t *m_buffer = NULL;

	// 图形转换对象
	struct SwsContext *m_pSwsCtx = NULL;

	// 音频转换对象
	struct SwrContext *m_pASwrCtx = NULL;

	SDLVideoWnd & m_textuer;

	std::thread *m_pThreadDecoder;

	bool m_exitThred = false;
	bool m_threadExited = false;

private:
	uint64_t m_dstChannelLayout = AV_CH_LAYOUT_STEREO;
	int m_nbSamples = 1024;
	AVSampleFormat m_dstSampleFmt = AV_SAMPLE_FMT_S16;
	int m_dstSampleRate = 44100;
	int m_dstChannels = 2;

public:
	FFAVDecoder(SDLVideoWnd &texture, AVPixelFormat fmt = AV_PIX_FMT_YUV420P);
	~FFAVDecoder();

public:
	// 播放流
	bool Init(std::string url);

private:
	void decoding(int start = 0);

	// 获取格式信息
	bool GetFormatContext(std::string url);

	// 获取流
	int FindStream(AVMediaType type);

	bool InitVedio();
	bool InitAudio();

private:
	// 解码媒体文件
	static int Run(FFAVDecoder* decoder, int start);

	// 取系统当前时间，毫秒
	static int64_t CurrentTime();

	// 时长
	int m_duration;

	// 解码播放状态
	FFDecoderStatus m_status;
public:
	// 播放视频
	bool Play(int start);

	// 暂停
	void Pause();

	// 继续播放
	void Resume();

	// 结束播放
	void Stop();

	// 播放状态
	FFDecoderStatus Status();

	// 正在播放？
	bool IsPlaying();

	// 暂停？
	bool IsPuased();

	// 结束？
	bool IsStopped();

private:
	void ExitThread();

};

#endif