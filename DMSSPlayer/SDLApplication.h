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
	// Ĭ��Flip���
	static const int FLIP_FREQUENCY = 1;

	// �˳�Flip�̱߳�־
	static bool terminalFlag;

	// Flip�߳�
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