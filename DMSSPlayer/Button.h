#ifndef __DMSS_BUTTON_H
#define __DMSS_BUTTON_H

#include "Component.h"
#include <string>

namespace dmss
{
	namespace sdl
	{
		class Button : public Component
		{
		private:
			SDL_Texture *m_pTexture = NULL;
			SDL_Surface *m_pPicture = NULL;
			SDL_Texture *m_pText = NULL;

			// 按钮文本
			std::string m_text;

			// 记录按钮normal和focus状态图片位置
			SDL_Rect m_rtNormal;
			SDL_Rect m_rtFocus;

			// 记录鼠标是否在按钮上
			bool m_isMotion = false;

			bool m_isChange = false;

		public:
			~Button();
			Button(IWindow* parent, std::string text, std::string bkgFileName, int x = 0, int y = 0, int w = 80, int h = 35);

		public:
			void Flip();

			// 抠色
			void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag = SDL_TRUE);

			// 处理SDL事件，返回false继续传递事件，反之处理完事件
			bool HandleEvent(SDL_Event &event);

			void Destroy();
		private:
			// 更新贴图
			void Update(void *data, int size);

		protected:
			// 是否需要重绘
			bool Changed();

			// 取控件贴图 
			SDL_Texture * DisplayTexture();
		};
	}
}
#endif
