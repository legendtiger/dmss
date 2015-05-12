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
	static const int DEFAULT_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

public:
	SDLWindow(std::string title, int w = DEFAULT_WIDTH, int h = DEFAULT_HIGHT, Uint32 flags = DEFAULT_FLAGS);
	SDLWindow(int w = DEFAULT_WIDTH, int h = DEFAULT_HIGHT, Uint32 flags = DEFAULT_FLAGS);
	~SDLWindow();

private:
	// SDL����
	SDL_Window *m_pWindow = NULL;

	// SDL Renderer
	SDL_Renderer *m_pRenderer = NULL;

	// �Ƿ�ˢ�´���
	bool m_isChanged = true;

	float m_scaleX = 1.0;
	float m_scaleY = 1.0;	

	// ���ڱ���
	std::string m_title;

	// ���ڳߴ�
	int m_width;
	int m_height;

	// ���ڱ�־
	Uint32 m_flags;

	// ������ʾ����
	std::vector < SDLItemBase*> m_items;

public:
	// �ƶ�����
	void Move(int x, int y);

	// �̳�SDLObject��HandleEvent����
	bool HandleEvent(SDL_Event &event);

	// �̳�SDLObject��Clone����
	SDLWindow & Clone();

	// ˢ�´���
	void Flip();

	// ��ȡ��ʾ����
	SDL_Rect GetRect();

	// ֪ͨ�����ͷ���Դ
	void Clean();

	virtual void show();

	float GetScaleX();
	float GetScaleY();
	SDL_Window* GetWindow();

	int Width();
	int Height();

	// ������Ԫ��
	friend class SDLItemBase;

protected:
	// �����ʾ����
	bool Add(SDLItemBase *item);

	virtual void Create(int w, int h);

private:
	// �Ӵ����Ƿ�ı䣬��Ҫ�ػ�
	bool ItemIsChanged();	
};

#endif