#ifndef __DMSS_UTILITY_H
#define __DMSS_UTILITY_H

#include <string>
#include "ImportSDL.h"

namespace dmss
{
	namespace sdl
	{
		class SDLUtil
		{
		private:
			SDLUtil();
		public:
			~SDLUtil();

		private:
			// SDL最常用的初始化参数组合
			static const Uint32 SDL_INIT_NORMAL = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;

		public:

			// 初始化SDL
			static void InitSDL(Uint32 flag = SDL_INIT_NORMAL);
			static void InitSubSystem(Uint32 flag);

			// 关闭SDL
			static void CloseSDL();

			// 获取SDL错误
			static std::string GetError();
		};
	}
}
#endif
