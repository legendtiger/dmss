#ifndef __DMSS_BUTTON_H
#define __DMSS_BUTTON_H

#include "SDLItemBase.h"
class SDLButton : public SDLItemBase
{
private:
	SDL_Texture *m_pTexture = NULL;
	SDL_Surface *m_pPicture = NULL;
	SDL_Texture *m_pText = NULL;

	// 按钮文本
	std::string m_text;

	// 记录按钮normal和focus状态图片位置
	SDL_Rect m_rtNormal;
	SDL_Rect m_rtFocus;

	// 记录鼠标是否在按钮上
	bool m_isMotion = false;

public:
	~SDLButton();
	SDLButton(SDLWindow& parent, std::string text, std::string bkgFileName, int x=0, int y=0, int w=80, int h=35);

public:
	void Flip();

	// 抠色
	void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag = SDL_TRUE);

	// 处理SDL事件，返回false继续传递事件，反之处理完事件
	bool HandleEvent(SDL_Event &event);

protected:
	SDL_Texture * DisplayTexture();

	bool Changed();
};
#endif
