#ifndef __DMSS_ITEM_BASE_H
#define __DMSS_ITEM_BASE_H

#include "SDLCommon.h"
#include "SDLObject.h"

class SDLWindow;

class SDLItemBase :	public SDLObject
{
private:
	// 显示位置大小
	SDL_Rect m_rt;

	// 渲染器
	SDLWindow *m_pParent = NULL;

public:
	virtual ~SDLItemBase();
	SDLItemBase(SDLWindow& parent, int x=0, int y=0, int w=0, int h=0);

	// 设置item位置
	void SetPosition(int x, int y);

	// 设置item大小
	void SetSize(int w, int h);

	// 刷新item
	virtual void Flip();

	// 抠色
	virtual void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag = SDL_TRUE);

	// 获取item位置大小
	SDL_Rect GetRect();

	virtual bool PointInItem(int x, int y);
protected:
	SDL_Renderer * GetRenderer();

	virtual SDL_Texture * DisplayTexture()=0;
};

#endif