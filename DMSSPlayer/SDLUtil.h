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
	}
}
#endif
