#include "SDLUtil.h"

SDLUtil::SDLUtil()
{
}

SDLUtil::~SDLUtil()
{
}

void SDLUtil::InitSDL(Uint32 flag)
{
	static bool bInitialized = false;
	if (!bInitialized)
	{
		bInitialized = true;
		//SDL_setenv("SDL_VIDEODRIVER", "directx", false);
		SDL_Init(flag);
	}
}

void SDLUtil::InitSubSystem(Uint32 flag)
{
	SDL_InitSubSystem(flag);
}

void SDLUtil::CloseSDL()
{
	SDL_Quit();
}

std::string SDLUtil::GetError()
{
	return SDL_GetError();
}