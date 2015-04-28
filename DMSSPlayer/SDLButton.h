#ifndef __DMSS_BUTTON_H
#define __DMSS_BUTTON_H

#include "SDLItemBase.h"
class SDLButton : public SDLItemBase
{
private:
	SDL_Texture *m_pTexture = NULL;

public:
	~SDLButton();
	SDLButton(SDLWindow& parent, std::string text, std::string bkgFileName, int x=0, int y=0, int w=80, int h=35);
};
#endif
