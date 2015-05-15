#ifndef __DMSS_ITEM_BASE_H
#define __DMSS_ITEM_BASE_H

#include "ImportSDL.h"
#include "Object.h"

namespace dmss
{
	namespace sdl
	{
		class IWindow;

		class Component : public Observable
		{
		private:
			// 显示位置大小
			SDL_Rect m_rt;

			// 渲染器
			IWindow *m_pParent;

		public:
			virtual ~Component();
			Component(IWindow* parent, int x = 0, int y = 0, int w = 0, int h = 0);

			// 设置item位置
			void SetPosition(int x, int y);

			// 设置item大小
			void SetSize(int w, int h);

			// 刷新item
			virtual void Flip();

			// 抠色
			virtual void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag = SDL_TRUE);

			// 获取item位置大小
			SDL_Rect GetRect();

			virtual bool PointInItem(int x, int y);

			// 获取父窗口
			IWindow *GetParent();

			virtual bool Changed() = 0;

			// 处理SDL事件，返回false继续传递事件，反之处理完事件
			virtual bool HandleEvent(SDL_Event &event) = 0;

		protected:

			// 控件贴图对象
			virtual SDL_Texture * DisplayTexture() = 0;
		};
	}
}
#endif