#include "Picture.h"

using namespace dmss::sdl;

Picture::Picture(IWindow* parent, std::string fileName, int x, int y)
:Component(parent, x, y, 0, 0)
{
	SDL_Surface *bmp = IMG_Load(fileName.c_str());
	if (bmp == NULL)
	{
		throw Error(SDL_GetError());
	}
	this->SetSize(bmp->w, bmp->h);
	this->m_pTexture = SDL_CreateTexture(GetParent()->GetRenderer(), bmp->format->format, SDL_TEXTUREACCESS_STATIC, bmp->w, bmp->h);
	SDL_UpdateTexture(this->m_pTexture, NULL, bmp->pixels, bmp->pitch);
}

Picture::~Picture()
{
}

bool Picture::HandleEvent(SDL_Event &event)
{
	return false;
}

SDL_Texture * Picture::DisplayTexture()
{
	return this->m_pTexture;
}

bool Picture::Changed()
{
	return false;
}
