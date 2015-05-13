#include "SDLItemBase.h"

SDLItemBase::~SDLItemBase()
	{
	}

SDLItemBase::SDLItemBase(dmss::IWindow* parent, int x, int y, int w, int h)
	{
		if (parent == NULL)
		{
			throw SDLError("����Ϊ�ؼ�ָ�������ڣ�");
		}
		m_pParent = parent;

		this->m_rt.x = x;
		this->m_rt.y = y;
		this->m_rt.w = w;
		this->m_rt.h = h;

		m_pParent->Add(this);
	}

	// ����itemλ��
	void SDLItemBase::SetPosition(int x, int y)
	{
		this->m_rt.x = x;
		this->m_rt.y = y;
	}

	// ����item��С
	void SDLItemBase::SetSize(int w, int h)
	{
		this->m_rt.w = w;
		this->m_rt.h = h;
	}


	SDL_Rect SDLItemBase::GetRect()
	{
		return this->m_rt;
	}

	void SDLItemBase::colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag)
	{

	}

	bool SDLItemBase::PointInItem(int x, int y)
	{
		// �ж��Ƿ��ڰ�ť��
		SDL_Rect rt = this->GetRect();
		if ((rt.x < x && (rt.x + rt.w)>x) && (rt.y < y && (rt.y + rt.h)>y))
		{
			return true;
		}

		return false;
	}

	void SDLItemBase::Flip()
	{
		if (Changed())
		{
			SDL_RenderCopy(m_pParent->GetRenderer(), this->DisplayTexture(), NULL, &this->GetRect());
		}
	}

	dmss::IWindow *SDLItemBase::GetParent()
	{
		return this->m_pParent;
	}
