#ifndef __IMPORT_SDL_H
#define __IMPORT_SDL_H


#ifdef __cplusplus
extern "C"
{
#endif
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#ifdef __cplusplus
}
#endif

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_ttf.lib")

#endif