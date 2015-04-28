#ifndef __DMSS_PICTURE_H
#define __DMSS_PICTURE_H

#include "SDLItemBase.h"
class SDLPicture : public SDLItemBase
{
private:
	SDL_Texture *m_pTexture = NULL;

public:
	SDLPicture(SDLWindow& parent,std::string fileName, int x = 0, int y = 0);
	~SDLPicture();

public:
	void Flip();
};

#endif