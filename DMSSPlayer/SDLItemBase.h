#ifndef __DMSS_ITEM_BASE_H
#define __DMSS_ITEM_BASE_H

#include "SDLCommon.h"
#include "SDLObject.h"

class SDLWindow;

class SDLItemBase :	public SDLObject
{
private:
	// ��ʾλ�ô�С
	SDL_Rect m_rt;

	// ��Ⱦ��
	SDLWindow *m_pParent = NULL;

public:
	virtual ~SDLItemBase();
	SDLItemBase(SDLWindow& parent, int x=0, int y=0, int w=0, int h=0);

	// ����itemλ��
	void SetPosition(int x, int y);

	// ����item��С
	void SetSize(int w, int h);

	// ˢ��item
	virtual void Flip();

	// ��ɫ
	virtual void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag = SDL_TRUE);

	// ��ȡitemλ�ô�С
	SDL_Rect GetRect();

	virtual bool PointInItem(int x, int y);
protected:
	SDL_Renderer * GetRenderer();

	virtual SDL_Texture * DisplayTexture()=0;
};

#endif