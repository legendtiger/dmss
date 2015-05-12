#include "SDLApplication.h"
#include "SDLCommon.h"
#include "SDLUtil.h"

bool SDLApplication::terminalFlag;

SDLApplication::SDLApplication(SDLWindow *mainWnd, int flipFrequency)
:m_pMain(mainWnd), m_flipFrequency(flipFrequency)
{
	SDLUtil::InitSDL();
	this->m_pMain->show();
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
void SDLApplication::FlipCallback(SDLWindow * window, int frequency)
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
}


// ���ٶ���
void SDLApplication::Destroy()
{
	this->m_pMain->Clean();
}
