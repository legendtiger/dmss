#include "SDLVideoWnd.h"
#include "SDLWindow.h"


SDLVideoWnd::SDLVideoWnd(SDLWindow& parent, int x, int y, int w, int h)
:SDLItemBase(parent, x, y, w, h)
{
}

SDLVideoWnd::~SDLVideoWnd()
{
	SDL_DestroyTexture(m_pVTexture);
}

void SDLVideoWnd::UpdateFrame(void * pixels, int pitch)
{
	SDL_UpdateTexture(this->m_pVTexture, NULL, pixels, pitch);	
	m_isChanged = true;
}

void SDLVideoWnd::InitVideo(int w, int h, Uint32 format)
{
	this->SetSize(w, h);
	m_pVTexture = SDL_CreateTexture(this->GetRenderer(), format, SDL_TEXTUREACCESS_STREAMING, w, h);
	if (m_pVTexture == NULL)
	{
		throw SDLError();
	}
}

SDL_Texture * SDLVideoWnd::DisplayTexture()
{
	return this->m_pVTexture;
}

bool SDLVideoWnd::HandleEvent(SDL_Event &event)
{	
	return false;
}

void SDLVideoWnd::Flip()
{
	SDL_RenderCopy(this->GetRenderer(), this->m_pVTexture, NULL, &this->GetParent()->GetRect());
	m_isChanged = false;
}

bool SDLVideoWnd::Changed()
{
	return m_isChanged;
}