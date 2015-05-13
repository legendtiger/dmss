#include "SDLPicture.h"

using dmss::IWindow;

	SDLPicture::SDLPicture(IWindow* parent, std::string fileName, int x, int y)
		:SDLItemBase(parent, x, y)
	{
		SDL_Surface *bmp = IMG_Load(fileName.c_str());
		if (bmp == NULL)
		{
			throw SDLError();
		}
		this->SetSize(bmp->w, bmp->h);
		this->m_pTexture = SDL_CreateTexture(GetParent()->GetRenderer(), bmp->format->format, SDL_TEXTUREACCESS_STATIC, bmp->w, bmp->h);
		SDL_UpdateTexture(this->m_pTexture, NULL, bmp->pixels, bmp->pitch);
	}

	SDLPicture::~SDLPicture()
	{
	}

	bool SDLPicture::HandleEvent(SDL_Event &event)
	{
		return false;
	}

	SDL_Texture * SDLPicture::DisplayTexture()
	{
		return this->m_pTexture;
	}

	bool SDLPicture::Changed()
	{
		return false;
	}
