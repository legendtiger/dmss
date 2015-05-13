#ifndef __DMSS_APPLICATION_H
#define __DMSS_APPLICATION_H

#include <thread>
#include "IWindow.h"
#include "SDLButton.h"
#include "SDLVideoWnd.h"
#include "FFAVDecoder.h"

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
	SDLButton *m_pButton = NULL;
	SDLVideoWnd *m_videoWnd = NULL;
	FFAVDecoder *m_avDecoder = NULL;

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