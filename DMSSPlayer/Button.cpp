#include "Button.h"
#include "SDL_rect.h"
#include "IWindow.h"

using namespace dmss::sdl;

Button::~Button()
{
	SDL_DestroyTexture(this->m_pTexture);
	SDL_FreeSurface(this->m_pPicture);
	SDL_DestroyTexture(this->m_pText);
}

Button::Button(IWindow* parent, std::string text, std::string bkgFileName, int x, int y, int w, int h)
:Component(parent, x, y, w, h), m_text(text)
{
	// 导入按钮图片
	this->m_pPicture = IMG_Load(bkgFileName.c_str());
	if (this->m_pPicture == NULL)
	{
		throw Error(SDL_GetError());
	}

	SDL_zero(m_rtNormal);
	this->m_rtNormal.w = this->m_pPicture->w / 2;
	this->m_rtNormal.h = this->m_pPicture->h;

	SDL_zero(m_rtFocus);
	this->m_rtFocus = this->m_rtNormal;
	this->m_rtFocus.x = this->m_rtNormal.w + 1;

	SDL_Renderer *pRender = GetParent()->GetRenderer();
	// 创建按钮文字贴图
	SDL_Color color = { 0, 0, 0 };
	TTF_Font *pFont = TTF_OpenFont("k:/JavaStudy/test.ttf", 20);
	SDL_Surface *sfText = TTF_RenderText_Solid(pFont, this->m_text.c_str(), color);
	this->m_pText = SDL_CreateTextureFromSurface(pRender, sfText);
	SDL_FreeSurface(sfText);

	// 创建背景贴图
	this->m_pTexture = SDL_CreateTextureFromSurface(pRender, this->m_pPicture);
	if (this->m_pTexture == NULL)
	{
		throw Error(SDL_GetError());
	}
}

// 刷新显示
void Button::Flip()
{
	SDL_Rect &rt = m_isMotion ? this->m_rtFocus : this->m_rtNormal;
	SDL_RenderCopy(GetParent()->GetRenderer(), this->m_pTexture, &rt, &this->GetRect());
	SDL_RenderCopy(GetParent()->GetRenderer(), this->m_pText, NULL, &this->GetRect());
	m_isChange = false;
}


bool Button::Changed()
{
	return m_isChange;
}

// 设置去除色
void Button::colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag)
{
	// 设置去除色
	Uint32 key = SDL_MapRGB(this->m_pPicture->format, r, g, b);
	SDL_SetColorKey(this->m_pPicture, flag, key);

	// 重新创建贴图
	SDL_DestroyTexture(this->m_pTexture);
	this->m_pTexture = SDL_CreateTextureFromSurface(GetParent()->GetRenderer(), this->m_pPicture);
	if (this->m_pTexture == NULL)
	{
		throw Error(SDL_GetError());
	}
}

bool Button::HandleEvent(SDL_Event &event)
{
	int x = event.button.x;
	int y = event.button.y;

	// 判断是否在按钮内
	if (!this->PointInItem(x, y))
	{
		m_isMotion = false;
		m_isChange = true;
		return false;
	}

	//throw SDLError();
	// 处理按钮内事件
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		m_isMotion = true;
		m_isChange = true;
		break;

	case SDL_MOUSEBUTTONUP:
		this->Update(NULL);
		break;

	default:
		break;
	}

	return true;
}

SDL_Texture * Button::DisplayTexture()
{
	return this->m_pTexture;
}
