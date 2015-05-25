#include "VideoRender.h"
#include "IWindow.h"

using namespace dmss::sdl;

VideoRender::VideoRender(IWindow* parent, int x, int y, int w, int h)
:Component(parent, x, y, w, h)
{
	
}

VideoRender::~VideoRender()
{
	SDL_DestroyTexture(m_pVTexture);
}

bool VideoRender::Changed()
{
	return m_isChanged;
}

// ����SDL�¼�������false���������¼�����֮�������¼�
bool VideoRender::HandleEvent(SDL_Event &event)
{
	return false;
}

// �ؼ���ͼ����
SDL_Texture * VideoRender::DisplayTexture()
{
	return m_pVTexture;
}

void VideoRender::Update(void *data, int size)
{
	// ������ͼ
	SDL_UpdateTexture(this->m_pVTexture, NULL, data, size);
	m_isChanged = true;
}

void VideoRender::InitTexture(int w, int h, Uint32 format)
{
	//fprintf(stdout, "w = %d, h= %d \n", w, h);
	m_pVTexture = SDL_CreateTexture(GetParent()->GetRenderer(), format, SDL_TEXTUREACCESS_STREAMING, w, h);
	if (m_pVTexture == NULL)
	{
		throw Error(SDL_GetError());
	}
}

void VideoRender::Flip()
{
	SDL_Rect rt = GetRect();
	SDL_RenderCopy(GetParent()->GetRenderer(), m_pVTexture, NULL, &rt);
	m_isChanged = false;
}