#ifndef __DMSS_APPLICATION_H
#define __DMSS_APPLICATION_H

#include <thread>
#include "IWindow.h"
#include "Button.h"
#include "Video.h"
#include "AVDecoder.h"

using namespace dmss::sdl;
using namespace dmss::ffmpeg;

class SDLApplication
{
private:
	// 默认Flip间隔
	static const int FLIP_FREQUENCY = 1;

	// 退出Flip线程标志
	static bool terminalFlag;

	// Flip线程
	std::thread *flipThread = NULL;

	int m_flipFrequency;

	IWindow *m_pMain = NULL;
	Button *m_pButton = NULL;
	Video *m_videoWnd = NULL;
	AVDecoder *m_avDecoder = NULL;

public:
	SDLApplication(int flipFrequency = FLIP_FREQUENCY);
	~SDLApplication();

public:
	// 开始应用程序
	void Execute();

private:
	// 应用程序初始化
	void Initialize();

	// 消息循环
	void EventLoop();

	// 销毁对象
	void Destroy();

private:
	// 显示线程执行函数
	static void FlipCallback(IWindow * window, int frequency);
};

#endif