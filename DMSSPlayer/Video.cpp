#include "Video.h"
#include "SDLWindow.h"

using namespace dmss::sdl;

Video::Video(IWindow* parent, int x, int y, int w, int h)
:Component(parent, x, y, w, h)
, m_stopFlag(false)
, m_decodeFinished(false)
{
}

Video::~Video()
{
	SDL_DestroyTexture(m_pVTexture);
}

void Video::Update(void * pixels, int pitch)
{
	SDL_UpdateTexture(this->m_pVTexture, NULL, pixels, pitch);
	m_isChanged = true;
}

void Video::InitVideo(int w, int h, Uint32 format)
{
	this->SetSize(w, h);
	m_pVTexture = SDL_CreateTexture(GetParent()->GetRenderer(), format, SDL_TEXTUREACCESS_STREAMING, w, h);
	if (m_pVTexture == NULL)
	{
		throw Error(SDL_GetError());
	}
}

SDL_Texture * Video::DisplayTexture()
{
	return this->m_pVTexture;
}

bool Video::HandleEvent(SDL_Event &event)
{
	return false;
}

void Video::Flip()
{
	try
	{
		SDL_Rect rt = this->GetParent()->GetRenderRect();
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
		SDL_RenderCopy(GetParent()->GetRenderer(), this->m_pVTexture, NULL, &rt);
		m_isChanged = false;
	}
	catch (...)
	{
		throw Error(SDL_GetError());
	}

}

bool Video::Changed()
{
	return m_isChanged;
}

void Video::Clean()
{
	m_stopFlag = true;
	while (!m_decodeFinished)
	{
		SDL_Delay(10);
	}
}

// 停止播放
bool Video::CanStop()
{
	return m_stopFlag;
}


// 解码线程完成或终止
void Video::DecodeFinished()
{
	m_stopFlag = false;
	m_decodeFinished = true;
}


// 初始化音频
void Video::InitAudio(int freq, int channels, int samples)
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
		throw Error(SDL_GetError());
	}

	fprintf(stdout, "freq相等？%d = %d \n", wanted_spec.freq, spec.freq);
	fprintf(stdout, "format相等? %d = %d \n", wanted_spec.format, spec.format);
	fprintf(stdout, "channels相等？%d = %d \n", wanted_spec.channels, spec.channels);
	fprintf(stdout, "samples相等？%d = %d \n", wanted_spec.samples, spec.samples);

	SDL_PauseAudio(0);
}

void Video::Destroy()
{

}
