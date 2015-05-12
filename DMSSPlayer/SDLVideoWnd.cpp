#include "SDLVideoWnd.h"
#include "SDLWindow.h"


SDLVideoWnd::SDLVideoWnd(SDLWindow& parent, int x, int y, int w, int h)
:SDLItemBase(parent, x, y, w, h)
, m_stopFlag(false)
, m_decodeFinished(false)
{
}

SDLVideoWnd::~SDLVideoWnd()
{
	SDL_DestroyTexture(m_pVTexture);
}

void SDLVideoWnd::UpdateFrame(void * pixels, int pitch)
{	
	SDL_UpdateTexture(this->m_pVTexture, NULL, pixels, pitch);	
	m_isChanged = true;
}

void SDLVideoWnd::InitVideo(int w, int h, Uint32 format)
{
	this->SetSize(w, h);
	m_pVTexture = SDL_CreateTexture(this->GetRenderer(), format, SDL_TEXTUREACCESS_STREAMING, w, h);
	if (m_pVTexture == NULL)
	{
		throw SDLError();
	}	
}

SDL_Texture * SDLVideoWnd::DisplayTexture()
{
	return this->m_pVTexture;
}

bool SDLVideoWnd::HandleEvent(SDL_Event &event)
{	
	return false;
}

void SDLVideoWnd::Flip()
{
	try
	{
		SDL_Rect rt = this->GetParent()->GetRect();
		/*
		int width = rt.w / this->GetParent()->GetScaleX();
		int height = rt.h / this->GetParent()->GetScaleY();
		fprintf(stdout, "scalex = %0.3f, height = %0.3f ", GetParent()->GetScaleX(), GetParent()->GetScaleY());
		fprintf(stdout, "width = %d, height = %d\n", width, height);
		rt.x = (rt.w - width) / 2;
		rt.y = (rt.h - height) / 2;
		rt.w = width;
		rt.h = height;//*/
		//SDL_RenderSetScale(this->GetRenderer(), this->GetParent()->GetScaleX(), this->GetParent()->GetScaleY());
		//SDL_RenderSetLogicalSize(this->GetRenderer());
		SDL_RenderCopy(this->GetRenderer(), this->m_pVTexture, NULL, &rt);
		m_isChanged = false;
	}
	catch (...)
	{
		throw SDLError();
	}
	
}

bool SDLVideoWnd::Changed()
{
	return m_isChanged;
}

void SDLVideoWnd::Clean()
{
	m_stopFlag = true;
	while (!m_decodeFinished)
	{
		SDL_Delay(10);
	}
}

// ֹͣ����
bool SDLVideoWnd::CanStop()
{
	return m_stopFlag;
}


// �����߳���ɻ���ֹ
void SDLVideoWnd::DecodeFinished()
{
	m_stopFlag = false;
	m_decodeFinished = true;
}


// ��ʼ����Ƶ
void SDLVideoWnd::InitAudio(int freq, int channels, int samples)
{	
	SDL_AudioSpec wanted_spec, spec;
	wanted_spec.freq = freq;
	wanted_spec.format = AUDIO_U16SYS;
	wanted_spec.channels = channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = samples;
	wanted_spec.callback = NULL;
	wanted_spec.userdata = NULL;

	if (SDL_OpenAudio(&wanted_spec, &spec) < 0)
	{
		throw SDLError();
	}

	fprintf(stdout, "freq��ȣ�%s\n", wanted_spec.freq == spec.freq ? "true" : "false");
	fprintf(stdout, "format���%d=%d��%s\n", wanted_spec.format, spec.format, wanted_spec.format == spec.format ? "true" : "false");
	fprintf(stdout, "channels��ȣ�%s\n", wanted_spec.channels == spec.channels ? "true" : "false");
	fprintf(stdout, "samples��ȣ�%s\n", wanted_spec.samples == spec.samples ? "true" : "false");

	SDL_PauseAudio(0);
}
