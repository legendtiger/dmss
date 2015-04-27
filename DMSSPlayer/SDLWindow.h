#pragma once

#include <string>
#ifdef __cplusplus
extern "C"
{
	#include "SDL.h"
}
#endif

class SDLWindow
{
public:
	static const int DEFAULT_WIDTH = 1024;
	static const int DEFAULT_HIGHT = 768;
	static const int DEFAULT_FLAGS = SDL_WINDOW_OPENGL;

public:
	SDLWindow(std::string title, int w = DEFAULT_WIDTH, int h = DEFAULT_HIGHT, Uint32 flags = DEFAULT_FLAGS);
	~SDLWindow();

private:
	// SDL窗口
	SDL_Window *m_pWindow = NULL;

	// SDL Renderer
	SDL_Renderer *m_pRenderer = NULL;
public:
	void show();
	// 窗口标题
	std::string m_title;
	// 窗口宽度

private:
	int m_width;
private:
	int m_height;
	Uint32 m_flags;
};

