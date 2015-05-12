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
	// 初始化
	this->Initialize();

	// 创建Flip线程
	this->flipThread = new std::thread(SDLApplication::FlipCallback, this->m_pMain, this->m_flipFrequency);
	this->flipThread->detach();

	// 开启消息循环
	EventLoop();

	// 结束
	Destroy();
}

void SDLApplication::EventLoop()
{
	// 开启消息循环
	SDL_Event event = { 0 };
	while (true)
	{
		// 提取消息
		SDL_PumpEvents();
		SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUIT, SDL_LASTEVENT);

		// 处理消息
		this->m_pMain->HandleEvent(event);

		// 判断是否结束程序
		if (event.type == SDL_QUIT)
		{
			SDLApplication::terminalFlag = true;

			// 等待线程退出
			while (SDLApplication::terminalFlag)
			{
				SDL_Delay(40);
			}
			break;
		}

		SDL_Delay(40);
	}
}


// 显示线程执行函数
void SDLApplication::FlipCallback(SDLWindow * window, int frequency)
{
	SDLApplication::terminalFlag = false;

	std::chrono::milliseconds dur(frequency);
	while (!SDLApplication::terminalFlag)
	{
		window->Flip();
		std::this_thread::sleep_for(dur);
	}

	// 表示线程已退出
	SDLApplication::terminalFlag = false;
}


// 应用程序初始化
void SDLApplication::Initialize()
{
}


// 销毁对象
void SDLApplication::Destroy()
{
	this->m_pMain->Clean();
}
