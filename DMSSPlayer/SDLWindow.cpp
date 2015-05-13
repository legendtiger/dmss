#include "SDLWindow.h"
#include "SDLVideoWnd.h"
#include "SDLButton.h"
#include "FFAVDecoder.h"

SDLWindow::SDLWindow(std::string title, int w, int h, Uint32 flags)
:IWindow()
, m_width(w)
, m_height(h)
, m_flags(flags)
{
	this->m_title = title;
	this->Create(this->m_width, this->m_height);
}

SDLWindow::SDLWindow(int w, int h, Uint32 flags)
:SDLWindow("", w, h, flags)
{
	
}

SDLWindow::~SDLWindow()
{
	SDL_DestroyRenderer(this->m_pRenderer);
	SDL_DestroyWindow(this->m_pWindow);
}

void SDLWindow::show()
{
	// 显示窗口
	SDL_RenderClear(this->m_pRenderer);
	SDL_RenderPresent(this->m_pRenderer);	
}

void SDLWindow::Create(int w, int h)
{
	if (this->m_pWindow != NULL)
	{
		SDL_DestroyRenderer(this->m_pRenderer);
		SDL_DestroyWindow(this->m_pWindow);
	}

	// 创建窗口和Renderer
	SDL_CreateWindowAndRenderer(w, h, this->m_flags, &this->m_pWindow, &this->m_pRenderer);
	SDL_SetWindowTitle(this->m_pWindow, this->m_title.c_str());
	SDL_SetRenderDrawColor(this->m_pRenderer, 0, 0, 0, 255);

	// make the scaled rendering look smoother.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(this->m_pRenderer, w, h);
}

// 移动窗口
void SDLWindow::Move(int x, int y)
{
	SDL_SetWindowPosition(this->m_pWindow, x, y);
}

bool SDLWindow::HandleEvent(SDL_Event &event)
{
	Uint32 windowID = SDL_GetWindowID(this->m_pWindow);
	switch (event.type)
	{
	case SDL_WINDOWEVENT:
		if (event.window.windowID == windowID)
		{
			switch (event.window.event)
			{
				// 此处发现改变窗口大小，视频不刷新需要修改源码中的代码，
				// 将SDL_OnWindowResized中的SDL_WINDOWEVENT_SIZE_CHANGED更改为SDL_WINDOWEVENT_RESIZED。
			case SDL_WINDOWEVENT_RESIZED:
			//case SDL_WINDOWEVENT_SIZE_CHANGED:			
			//case SDL_WINDOWEVENT_MAXIMIZED:
			//case SDL_WINDOWEVENT_MINIMIZED:
			//case SDL_WINDOWEVENT_RESTORED:
				//fprintf(stdout, "winWidth = %d, winHeight = %d\n", event.window.data1, event.window.data2);
				//this->m_width = event.window.data1;
				//this->m_height = event.window.data2;
				m_scaleX = event.window.data1 * 1.0 / m_width;
				m_scaleY = event.window.data2 * 1.0 / m_height;
				//SDL_SetWindowSize(m_pWindow, m_width, m_height);				
				SDL_RenderSetViewport(m_pRenderer, &this->GetRenderRect());
				SDL_RenderSetLogicalSize(m_pRenderer, m_width, m_height);
				m_isChanged = true;
				break;

			case SDL_WINDOWEVENT_CLOSE:
				event.type = SDL_QUIT;
				SDL_PushEvent(&event);
				break;

			default:
				break;
			}			
		}
		
		break;
	}
	bool flag = false;
	int len = this->m_items.size();
	for (int i = 0; i < len; i++)
	{
		flag |= this->m_items[i]->HandleEvent(event);
	}
	return flag;
}

// 刷新item
void SDLWindow::Flip()
{
	if (m_isChanged || ItemIsChanged())
	{
		// 显示窗口
		SDL_RenderClear(this->m_pRenderer);
		int len = this->m_items.size();
		for (int i = 0; i < len; i++)
		{
			this->m_items[i]->Flip();
		}
		SDL_RenderPresent(this->m_pRenderer);
		m_isChanged = false;
	}
}

bool SDLWindow::ItemIsChanged()
{
	bool changed = false;
	int len = this->m_items.size();
	for (int i = 0; i < len; i++)
	{
		changed |= this->m_items[i]->Changed();
	}
	return changed;
}
void SDLWindow::Destroy()
{
	int len = this->m_items.size();
	for (int i = 0; i < len; i++)
	{
		this->m_items[i]->Destroy();
	}
}

// 添加显示对象
bool SDLWindow::Add(SDLItemBase *item)
{
	int len = this->m_items.size();
	for (int i = 0; i < len; i++)
	{
		if (this->m_items.at(i) == item)
		{
			return false;
		}
	}
	this->m_items.push_back(item);
	return true;
}

SDL_Rect SDLWindow::GetRenderRect()
{
	SDL_Rect rt = { 0, 0, this->m_width, this->m_height };
	return rt;
}


float SDLWindow::GetScaleX()
{
	return m_scaleX;
}

float SDLWindow::GetScaleY()
{
	return m_scaleY;
}

SDL_Window* SDLWindow::GetWindow()
{
	return m_pWindow;
}

SDL_Renderer* SDLWindow::GetRenderer()
{
	return m_pRenderer;
}

void SDLWindow::Resize(int w, int h)
{
	SDL_SetWindowSize(m_pWindow, w, h);
	SDL_RenderSetLogicalSize(this->m_pRenderer, w, h);
}


int SDLWindow::Width()
{
	return m_width;
}


int SDLWindow::Height()
{
	return m_height;
}
