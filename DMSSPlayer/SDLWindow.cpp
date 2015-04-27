#include "SDLWindow.h"

SDLWindow::SDLWindow(std::string title, int w, int h, Uint32 flags)
: m_width(w)
, m_height(h)
, m_flags(flags)
{
	this->m_title = title;		
}

SDLWindow::~SDLWindow()
{
}


void SDLWindow::show()
{
	// 创建窗口和Renderer
	SDL_CreateWindowAndRenderer(this->m_width, this->m_height, this->m_flags, &this->m_pWindow, &this->m_pRenderer);
	SDL_SetWindowTitle(this->m_pWindow, this->m_title.c_str());
	SDL_SetRenderDrawColor(this->m_pRenderer, 0, 0, 0, 255);
	// make the scaled rendering look smoother.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  
	SDL_RenderSetLogicalSize(this->m_pRenderer, this->m_width, this->m_height);

	// 显示窗口
	SDL_RenderClear(this->m_pRenderer);
	SDL_RenderPresent(this->m_pRenderer);

	bool quitFlag = false;
	// 开启消息循环
	SDL_Event event = { 0 };
	while (!quitFlag)
	{
		SDL_Delay(100);
		SDL_PumpEvents();
		SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT);
		switch (event.type)
		{
		case SDL_QUIT:
			quitFlag = true;
			break;
		default:
			break;
		}
	}
}
