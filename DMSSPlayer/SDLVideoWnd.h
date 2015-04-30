#ifndef __DMSS_VIDEO_WND_H
#define __DMSS_VIDEO_WND_H

#include "SDLItemBase.h"

class SDLVideoWnd :	public SDLItemBase
{
private:
	SDL_Texture *m_pVTexture = NULL;

	bool m_isChanged = false;

public:
	SDLVideoWnd(SDLWindow& parent, int x = 0, int y = 0, int w = 800, int h = 600);
	~SDLVideoWnd();

public:
	// 更新帧数据
	void UpdateFrame(void * pixels, int pitch);

	// 初始化视频
	void InitVideo(int w, int h, Uint32 format = SDL_PIXELFORMAT_YV12);

public:
	bool HandleEvent(SDL_Event &event);

	void Flip();

protected:
	SDL_Texture * DisplayTexture();

	bool Changed();
};

#endif