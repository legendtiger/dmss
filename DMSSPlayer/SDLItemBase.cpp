#include "SDLItemBase.h"
#include "SDLWindow.h"

SDLItemBase::~SDLItemBase()
{
}

SDLItemBase::SDLItemBase(SDLWindow& parent)
{
	parent.Add(this);
	m_pParent = &parent;
}

// 设置item位置
void SDLItemBase::SetPosition(int x, int y)
{
	this->m_rt.x = x;
	this->m_rt.y = y;
}

// 设置item大小
void SDLItemBase::SetSize(int w, int h)
{
	this->m_rt.w = w;
	this->m_rt.h = h;
}

SDL_Renderer * SDLItemBase::GetRenderer()
{
	return this->m_pParent->m_pRenderer;
}

SDL_Rect SDLItemBase::GetRect()
{
	return this->m_rt;
}

void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag)
{

}
