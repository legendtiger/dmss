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
	SDLItemBase(SDLWindow& parent);

	// ����itemλ��
	void SetPosition(int x, int y);

	// ����item��С
	void SetSize(int w, int h);

	// ˢ��item
	virtual void Flip() = 0;

	// ��ɫ
	void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag);

	// ��ȡitemλ�ô�С
	SDL_Rect GetRect();
protected:
	SDL_Renderer * GetRenderer();	
};

#endif