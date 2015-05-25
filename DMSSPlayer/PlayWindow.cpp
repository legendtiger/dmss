#include "PlayWindow.h"

using namespace dmss::sdl;

PlayWindow::PlayWindow(std::string title, int w, int h, Uint32 flags)
:IWindow(w, h), m_title(title), m_flags(flags)
{
	// 创建窗口和Renderer
	SDL_CreateWindowAndRenderer(w, h, this->m_flags, &this->m_pWindow, &this->m_pRenderer);
	SDL_SetWindowTitle(this->m_pWindow, this->m_title.c_str());
	SDL_SetRenderDrawColor(this->m_pRenderer, 0, 0, 0, 255);

	// make the scaled rendering look smoother.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(this->m_pRenderer, w, h);

	// 显示窗口
	SDL_RenderClear(this->m_pRenderer);
	SDL_RenderPresent(this->m_pRenderer);

	m_pVideoRender = new VideoRender(this, 0, 0, w, h);
	m_player = new AVPlayer(m_pVideoRender);
	m_pUnpacker = new Unpacker(m_player);
}


PlayWindow::~PlayWindow()
{
	delete m_player;
	delete m_pVideoRender;

	SDL_DestroyRenderer(this->m_pRenderer);
	SDL_DestroyWindow(this->m_pWindow);
}

// 获取显示区域
SDL_Rect PlayWindow::GetRenderRect()
{
	SDL_Rect rt;
	SDL_zero(rt);
	rt.w = this->m_width;
	rt.h = this->m_height;
	return rt;
}

// 获取SDL窗口
SDL_Window* PlayWindow::GetWindow()
{
	return m_pWindow;
}

// 获取Renderer
SDL_Renderer* PlayWindow::GetRenderer()
{
	return m_pRenderer;
}

// 刷新窗口
void PlayWindow::Flip()
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

bool PlayWindow::ItemIsChanged()
{
	bool changed = false;
	int len = this->m_items.size();
	for (int i = 0; i < len; i++)
	{
		changed |= this->m_items[i]->Changed();
	}
	return changed;
}

// 调整窗口尺寸
void PlayWindow::Resize(int w, int h)
{
}

// 移动窗口
void PlayWindow::Move(int x, int y)
{
	SDL_SetWindowPosition(this->m_pWindow, x, y);
}

// 添加子控件
bool PlayWindow::Add(Component* child)
{
	int len = this->m_items.size();
	for (int i = 0; i < len; i++)
	{
		if (this->m_items.at(i) == child)
		{
			return false;
		}
	}
	this->m_items.push_back(child);
	return true;
}

// 处理SDL事件，返回false继续传递事件，反之处理完事件
bool PlayWindow::HandleEvent(SDL_Event &event)
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
//				m_isChanged = true;
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

void PlayWindow::Play(std::string filename)
{
	m_player->Open(filename, 0);
	m_player->Play();
}
