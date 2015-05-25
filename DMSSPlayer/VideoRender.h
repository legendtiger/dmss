#ifndef __DMSS_VIDEORENDER_H
#define __DMSS_VIDEORENDER_H

#include "Component.h"
namespace dmss
{
	namespace sdl
	{
		class VideoRender : public Component
		{
		public:
			VideoRender(IWindow* parent, int x, int y, int w, int h);
			~VideoRender();

		private:
			// 贴图对象
			SDL_Texture *m_pVTexture = NULL;

			// 是否更新了内容
			bool m_isChanged = false;

		public:
			bool Changed();

			// 处理SDL事件，返回false继续传递事件，反之处理完事件
			bool HandleEvent(SDL_Event &event);

			// 更新数据
			void Update(void *data, int size);

			// 初始化贴图
			void InitTexture(int w, int h, Uint32 format = SDL_PIXELFORMAT_YV12);

			void Flip();

		protected:
			// 控件贴图对象
			SDL_Texture * DisplayTexture();
		};
	}
}

#endif