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
	// SDL����
	SDL_Window *m_pWindow = NULL;

	// SDL Renderer
	SDL_Renderer *m_pRenderer = NULL;

	// �Ƿ�ˢ�´���
	bool m_isChanged = true;

public:
	void show();
	// ���ڱ���
	std::string m_title;

private:
	// ���ڳߴ�
	int m_width;
	int m_height;

	// ���ڱ�־
	Uint32 m_flags;

private:
	// ������ʾ����
	std::vector < SDLItemBase*> m_items;

public:
	// ��Ϣѭ��
	void EventLoop();

	// �ƶ�����
	void Move(int x, int y);

	// �̳�SDLObject��HandleEvent����
	bool HandleEvent(SDL_Event &event);

	// �̳�SDLObject��Clone����
	SDLWindow & Clone();

	// ˢ�´���
	void Flip();

	SDL_Rect GetRect();

	// ������Ԫ��
	friend class SDLItemBase;

protected:
	// �����ʾ����
	bool Add(SDLItemBase *item);

private:
	void Create(int w, int h);

	bool ItemIsChanged();
};

#endif