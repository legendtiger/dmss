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
		SDL_RenderCopy(this->GetRenderer(), this->m_pVTexture, NULL, &this->GetParent()->GetRect());
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

// 停止播放
bool SDLVideoWnd::CanStop()
{
	return m_stopFlag;
}


// 解码线程完成或终止
void SDLVideoWnd::DecodeFinished()
{
	m_stopFlag = false;
	m_decodeFinished = true;
}


// 初始化音频
void SDLVideoWnd::InitAudio(int freq, int format, int channels, int samples, void* userdata)
{	
	SDL_AudioSpec wanted_spec, spec;
	wanted_spec.freq = freq;
	wanted_spec.format = AUDIO_U16SYS;
	wanted_spec.channels = channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = samples;
	wanted_spec.callback = NULL;
	wanted_spec.userdata = userdata;

	if (SDL_OpenAudio(&wanted_spec, &spec) < 0)
	{
		throw SDLError();
	}

	fprintf(stdout, "freq相等？%s\n", wanted_spec.freq == spec.freq ? "true" : "false");
	fprintf(stdout, "format相等%d=%d？%s\n", wanted_spec.format, spec.format, wanted_spec.format == spec.format ? "true" : "false");
	fprintf(stdout, "channels相等？%s\n", wanted_spec.channels == spec.channels ? "true" : "false");
	fprintf(stdout, "samples相等？%s\n", wanted_spec.samples == spec.samples ? "true" : "false");

	SDL_PauseAudio(0);
}
