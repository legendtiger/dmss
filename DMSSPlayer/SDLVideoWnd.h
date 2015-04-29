#ifndef __DMSS_VIDEO_WND_H
#define __DMSS_VIDEO_WND_H

#include "SDLItemBase.h"

class SDLVideoWnd :	public SDLItemBase
{
private:
	SDL_Texture *m_pVTexture = NULL;

public:
	SDLVideoWnd(SDLWindow& parent, Uint32 format = SDL_PIXELFORMAT_YV12, int x = 0, int y = 0, int w = 0, int h = 0);
	~SDLVideoWnd();

public:
	// 更新帧数据
	void UpdateFrame(void * pixels, int pitch);
public:
	bool HandleEvent(SDL_Event &event);

protected:
	SDL_Texture * DisplayTexture();
};

#endif