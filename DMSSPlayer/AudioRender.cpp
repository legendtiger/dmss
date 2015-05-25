#include "AudioRender.h"
#include "ImportSDL.h"

using namespace dmss::sdl;

AudioRender::AudioRender()
{
}


AudioRender::~AudioRender()
{
}

void AudioRender::Update(void *data, int size)
{
//	fprintf(stdout, "int size = %d \n", size);
	if (SDL_QueueAudio(1, data, size) == -1)
	{
		fprintf(stdout, "SDL_QueueAudio failed! int size = %d \n", size);
	}
}

void AudioRender::InitAudio(int freq, int channels, int samples)
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

void AudioRender::Pause()
{
	SDL_PauseAudio(1);
}


void AudioRender::Stop()
{
	SDL_ClearQueuedAudio(1);
	SDL_CloseAudio();
}


void AudioRender::Play()
{
	SDL_PauseAudio(0);
}
