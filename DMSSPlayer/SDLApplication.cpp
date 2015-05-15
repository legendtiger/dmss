#include "SDLApplication.h"
#include "ImportSDL.h"
#include "SDLUtil.h"
#include "Video.h"
#include "Button.h"
#include "AVDecoder.h"
#include "SDLWindow.h"

bool SDLApplication::terminalFlag;

SDLApplication::SDLApplication(int flipFrequency)
:m_flipFrequency(flipFrequency)
{
	SDLUtil::InitSDL();
}


SDLApplication::~SDLApplication()
{
	SDLUtil::CloseSDL();
}


void SDLApplication::Execute()
{
	// ��ʼ��
	this->Initialize();

	// ����Flip�߳�
	this->flipThread = new std::thread(SDLApplication::FlipCallback, this->m_pMain, this->m_flipFrequency);
	this->flipThread->detach();

	// ������Ϣѭ��
	EventLoop();

	// ����
	Destroy();
}

void SDLApplication::EventLoop()
{
	// ������Ϣѭ��
	SDL_Event event = { 0 };
	while (true)
	{
		// ��ȡ��Ϣ
		SDL_PumpEvents();
		SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUIT, SDL_LASTEVENT);

		// ������Ϣ
		this->m_pMain->HandleEvent(event);

		// �ж��Ƿ��������
		if (event.type == SDL_QUIT)
		{
			SDLApplication::terminalFlag = true;

			m_pMain->Destroy();

			// �ȴ��߳��˳�
			while (SDLApplication::terminalFlag)
			{
				SDL_Delay(40);
			}
			break;
		}

		SDL_Delay(40);
	}
}


// ��ʾ�߳�ִ�к���
void SDLApplication::FlipCallback(IWindow * window, int frequency)
{
	SDLApplication::terminalFlag = false;

	std::chrono::milliseconds dur(frequency);
	while (!SDLApplication::terminalFlag)
	{
		window->Flip();
		std::this_thread::sleep_for(dur);
	}

	// ��ʾ�߳����˳�
	SDLApplication::terminalFlag = false;
}


// Ӧ�ó����ʼ��
void SDLApplication::Initialize()
{
	m_pMain = new SDLWindow("DMSS media player");
	m_videoWnd = new Video(m_pMain);
	m_pButton = new Button(m_pMain, "", "k:/javastudy/button.bmp");
	m_avDecoder = new AVDecoder(*m_videoWnd);
	if (m_avDecoder->Init("k:/javastudy/test.avi"))
	{
		m_avDecoder->Play(0);
	}
	//this->m_pMain->show();
}


// ���ٶ���
void SDLApplication::Destroy()
{
	this->m_pMain->Destroy();
}
