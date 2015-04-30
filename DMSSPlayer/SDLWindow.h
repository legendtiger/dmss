#ifndef __DMSS_WINDOW_H
#define __DMSS_WINDOW_H

#include "SDLCommon.h"
#include "SDLObject.h"
#include "SDLItemBase.h"

#include <string>
#include <vector>

class SDLWindow : public SDLObject
{
public:
	static const int DEFAULT_WIDTH = 1024;
	static const int DEFAULT_HIGHT = 768;
	static const int DEFAULT_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

public:
	SDLWindow(std::string title, int w = DEFAULT_WIDTH, int h = DEFAULT_HIGHT, Uint32 flags = DEFAULT_FLAGS);
	~SDLWindow();

private:
	// SDL窗口
	SDL_Window *m_pWindow = NULL;

	// SDL Renderer
	SDL_Renderer *m_pRenderer = NULL;

	// 是否刷新窗口
	bool m_isChanged = true;

public:
	void show();
	// 窗口标题
	std::string m_title;

private:
	// 窗口尺寸
	int m_width;
	int m_height;

	// 窗口标志
	Uint32 m_flags;

private:
	// 保存显示对象
	std::vector < SDLItemBase*> m_items;

public:
	// 消息循环
	void EventLoop();

	// 移动窗口
	void Move(int x, int y);

	// 继承SDLObject的HandleEvent方法
	bool HandleEvent(SDL_Event &event);

	// 继承SDLObject的Clone方法
	SDLWindow & Clone();

	// 刷新窗口
	void Flip();

	SDL_Rect GetRect();

	// 定义友元类
	friend class SDLItemBase;

protected:
	// 添加显示对象
	bool Add(SDLItemBase *item);

private:
	void Create(int w, int h);

	bool ItemIsChanged();
};

#endif