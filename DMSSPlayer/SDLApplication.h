#ifndef __DMSS_APPLICATION_H
#define __DMSS_APPLICATION_H

#include <thread>
#include "PlayWindow.h"
#include "Video.h"
#include "Button.h"
#include "AVDecoder.h"
#include "SDLWindow.h"

using namespace dmss::ffmpeg;
using namespace dmss::sdl;

class SDLApplication
{
private:
	// Ĭ��Flip���
	static const int FLIP_FREQUENCY = 10;

	// �˳�Flip�̱߳�־
	static bool terminalFlag;

	// Flip�߳�
	std::thread *flipThread = NULL;

	int m_flipFrequency;
	IWindow *m_pMain = NULL;

	PlayWindow *m_pLayMain = NULL;

	SDLWindow *m_pTestMain = NULL;

	Video *m_videoWnd = NULL;

	AVDecoder *m_avDecoder = NULL;

public:
	SDLApplication(int flipFrequency = FLIP_FREQUENCY);
	~SDLApplication();

public:
	// ��ʼӦ�ó���
	void Execute();

private:
	// Ӧ�ó����ʼ��
	void Initialize();

	// ��Ϣѭ��
	void EventLoop();

	// ���ٶ���
	void Destroy();

private:
	// ��ʾ�߳�ִ�к���
	static void FlipCallback(IWindow * window, int frequency);
};

#endif