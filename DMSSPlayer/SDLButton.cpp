#include "SDLButton.h"

SDLButton::~SDLButton()
{
}

SDLButton::SDLButton(SDLWindow& parent, std::string text, std::string bkgFileName, int x, int y, int w, int h)
:SDLItemBase(parent)
{
	SDL_Surface *bmp = SDL_LoadBMP(bkgFileName.c_str());
	if (bmp == NULL)
	{
		throw SDLError();
	}
	this->SetPosition(x, y);
	this->SetSize(w, h);
	this->m_pTexture = SDL_CreateTexture(this->GetRenderer(), bmp->format->format, SDL_TEXTUREACCESS_STATIC, bmp->w, bmp->h);
	SDL_UpdateTexture(this->m_pTexture, NULL, bmp->pixels, bmp->pitch);

}
