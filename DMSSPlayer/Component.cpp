#include "Component.h"
#include "IWindow.h"

using namespace dmss::sdl;

Component::~Component()
{
}

Component::Component(IWindow* parent, int x, int y, int w, int h)
{
	if (parent == NULL)
	{
		throw Error("����Ϊ�ؼ�ָ�������ڣ�");
	}
	m_pParent = parent;

	this->m_rt.x = x;
	this->m_rt.y = y;
	this->m_rt.w = w;
	this->m_rt.h = h;

	m_pParent->Add(this);
}

// ����itemλ��
void Component::SetPosition(int x, int y)
{
	this->m_rt.x = x;
	this->m_rt.y = y;
}

// ����item��С
void Component::SetSize(int w, int h)
{
	this->m_rt.w = w;
	this->m_rt.h = h;
}


SDL_Rect Component::GetRect()
{
	return this->m_rt;
}

void Component::colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag)
{

}

bool Component::PointInItem(int x, int y)
{
	// �ж��Ƿ��ڰ�ť��
	SDL_Rect rt = this->GetRect();
	if ((rt.x < x && (rt.x + rt.w)>x) && (rt.y < y && (rt.y + rt.h)>y))
	{
		return true;
	}

	return false;
}

void Component::Flip()
{
	if (Changed())
	{
		SDL_RenderCopy(m_pParent->GetRenderer(), this->DisplayTexture(), NULL, &this->GetRect());
	}
}

IWindow *Component::GetParent()
{
	return this->m_pParent;
}
