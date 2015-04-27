#pragma once

#include <string>
#ifdef __cplusplus
extern "C"
{
	#include "SDL.h"
}
#endif
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

class SDLUtil
{
private:
	SDLUtil();
public:
	~SDLUtil();

private:
	// SDL��õĳ�ʼ���������
	static const Uint32 SDL_INIT_NORMAL = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;

public:
	// ��ʼ��SDL
	static void InitSDL(Uint32 flag = SDL_INIT_NORMAL);
	static void InitSubSystem(Uint32 flag);

	// �ر�SDL
	static void CloseSDL();

	// ��ȡSDL����
	static std::string GetError();
};

