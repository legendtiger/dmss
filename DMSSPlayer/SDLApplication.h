#ifndef __DMSS_APPLICATION_H
#define __DMSS_APPLICATION_H

#include <thread>
#include "SDLWindow.h"

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

	SDLWindow *m_pMain = NULL;

public:
	SDLApplication(SDLWindow *mainWnd, int flipFrequency = FLIP_FREQUENCY);
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
	static void FlipCallback(SDLWindow * window, int frequency);	
};

#endif