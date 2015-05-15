#ifndef __DMSS_PICTURE_H
#define __DMSS_PICTURE_H

#include "Component.h"
#include "IWindow.h"
#include <string>
namespace dmss
{
	namespace sdl
	{
		class Picture : public Component
		{
		private:
			SDL_Texture *m_pTexture = NULL;

		public:
			Picture(IWindow* parent, std::string fileName, int x = 0, int y = 0);
			~Picture();

		public:
			bool HandleEvent(SDL_Event &event);

		protected:
			SDL_Texture * DisplayTexture();

			bool Changed();
		};
	}
}
#endif