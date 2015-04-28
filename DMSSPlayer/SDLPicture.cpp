#include "SDLPicture.h"

SDLPicture::SDLPicture(SDLWindow& parent, std::string fileName, int x, int y)
:SDLItemBase(parent)
{
	SDL_Surface *bmp = IMG_Load(fileName.c_str());
	if (bmp == NULL)
	{
		throw SDLError();
	}
	this->SetPosition(x, y);
	this->SetSize(bmp->w, bmp->h);
	this->m_pTexture = SDL_CreateTexture(this->GetRenderer(), bmp->format->format, SDL_TEXTUREACCESS_STATIC, bmp->w, bmp->h);
	SDL_UpdateTexture(this->m_pTexture, NULL, bmp->pixels, bmp->pitch);	
}

SDLPicture::~SDLPicture()
{
}

void SDLPicture::Flip()
{
	SDL_RenderCopy(this->GetRenderer(), this->m_pTexture, NULL, &this->GetRect());
}