#include "SDLButton.h"
#include "SDL_rect.h"

SDLButton::~SDLButton()
{
	SDL_DestroyTexture(this->m_pTexture);
	SDL_FreeSurface(this->m_pPicture);
	SDL_DestroyTexture(this->m_pText);
}

SDLButton::SDLButton(SDLWindow& parent, std::string text, std::string bkgFileName, int x, int y, int w, int h)
:SDLItemBase(parent, x, y, w, h), m_text(text)
{
	// ���밴ťͼƬ
	this->m_pPicture = IMG_Load(bkgFileName.c_str());
	if (this->m_pPicture == NULL)
	{
		throw SDLError();
	}

	this->m_rtNormal.w = this->m_pPicture->w / 2;
	this->m_rtNormal.h = this->m_pPicture->h;

	this->m_rtFocus = this->m_rtNormal;
	this->m_rtFocus.x = this->m_rtNormal.w + 1;

	// ������ť������ͼ
	SDL_Color color = { 0, 0, 0 };
	TTF_Font *pFont = TTF_OpenFont("k:/JavaStudy/test.ttf", 20);
	SDL_Surface *sfText = TTF_RenderText_Solid(pFont, this->m_text.c_str(), color);
	this->m_pText = SDL_CreateTextureFromSurface(this->GetRenderer(), sfText);
	SDL_FreeSurface(sfText);

	// ����������ͼ
	this->m_pTexture = SDL_CreateTextureFromSurface(this->GetRenderer(), this->m_pPicture);
	if (this->m_pTexture == NULL)
	{
		throw SDLError();
	}
}

// ˢ����ʾ
void SDLButton::Flip()
{
	SDL_Rect &rt = m_isMotion ? this->m_rtFocus : this->m_rtNormal;
	SDL_RenderCopy(this->GetRenderer(), this->m_pTexture, &rt, &this->GetRect());
	SDL_RenderCopy(this->GetRenderer(), this->m_pText, NULL, &this->GetRect());
}

SDL_Texture * SDLButton::DisplayTexture()
{
	return NULL;
}

bool SDLButton::Changed()
{
	return false;
}

// ����ȥ��ɫ
void SDLButton::colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag)
{
	// ����ȥ��ɫ
	Uint32 key = SDL_MapRGB(this->m_pPicture->format, r, g, b);
	SDL_SetColorKey(this->m_pPicture, flag, key);

	// ���´�����ͼ
	SDL_DestroyTexture(this->m_pTexture);
	this->m_pTexture = SDL_CreateTextureFromSurface(this->GetRenderer(), this->m_pPicture);
	if (this->m_pTexture == NULL)
	{
		throw SDLError();
	}
}

bool SDLButton::HandleEvent(SDL_Event &event)
{
	int x = event.button.x;
	int y = event.button.y;

	// �ж��Ƿ��ڰ�ť��
	if (!this->PointInItem(x, y))
	{
		m_isMotion = false;
		return false;
	}

	//throw SDLError();
	// ����ť���¼�
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		m_isMotion = true;
		break;

	case SDL_MOUSEBUTTONUP:
		this->Update(NULL);
		break;

	default:
		break;
	}

	return true;
}
