#include "SDLUtil.h"

using namespace dmss::sdl;

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

		SDL_Init(flag);

		// 初始化图片加载
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

		// 初始化ttf字体加载
		TTF_Init();
	}
}

void SDLUtil::InitSubSystem(Uint32 flag)
{
	SDL_InitSubSystem(flag);
}

void SDLUtil::CloseSDL()
{
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
}

std::string SDLUtil::GetError()
{
	return SDL_GetError();
}
