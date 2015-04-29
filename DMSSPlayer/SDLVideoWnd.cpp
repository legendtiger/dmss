#include "SDLVideoWnd.h"


SDLVideoWnd::SDLVideoWnd(SDLWindow& parent, Uint32 format, int x, int y, int w, int h)
:SDLItemBase(parent, x, y, w, h)
{
	m_pVTexture = SDL_CreateTexture(this->GetRenderer(),
		format, SDL_TEXTUREACCESS_STREAMING, w, h);
}


SDLVideoWnd::~SDLVideoWnd()
{
	SDL_DestroyTexture(m_pVTexture);
}

void SDLVideoWnd::UpdateFrame(void * pixels, int pitch)
{
	SDL_UpdateTexture(this->m_pVTexture, NULL, pixels, pitch);
}

SDL_Texture * SDLVideoWnd::DisplayTexture()
{
	return this->m_pVTexture;
}

bool SDLVideoWnd::HandleEvent(SDL_Event &event)
{
	return false;
}