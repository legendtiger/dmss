#ifndef __DMSS_PLAYER_H
#define __DMSS_PLAYER_H

#include "SDLWindow.h"

class DMSSPlayer : public SDLWindow
{
private:
	SDLWindow *m_pScreen = NULL;

	SDLWindow *m_pToolBar = NULL;
public:
	DMSSPlayer(std::string title, int w = DEFAULT_WIDTH, int h = DEFAULT_HIGHT, Uint32 flags = DEFAULT_FLAGS);
	~DMSSPlayer();

public:
	void Show();

protected:
	void Create();
};

#endif