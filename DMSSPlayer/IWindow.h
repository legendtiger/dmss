#ifndef __DMSS_IWINDOW_H
#define __DMSS_IWINDOW_H

#include "Object.h"
#include "Component.h"

namespace dmss
{
	namespace sdl
	{
		class IWindow : public Object
		{
		public:
			IWindow(int width, int height):m_width(width),m_height(height){};
			virtual ~IWindow(){};

		protected:
			int m_width;
			int m_height;

		public:
			// 获取显示区域
			virtual SDL_Rect GetRenderRect() = 0;

			// 获取SDL窗口
			virtual SDL_Window* GetWindow() = 0;

			// 获取Renderer
			virtual SDL_Renderer* GetRenderer() = 0;

			// 刷新窗口
			virtual void Flip() = 0;

			// 调整窗口尺寸
			virtual void Resize(int w, int h) = 0;

			// 移动窗口
			virtual void Move(int x, int y) = 0;

			// 添加子控件
			virtual bool Add(Component* child) = 0;

			// 处理SDL事件，返回false继续传递事件，反之处理完事件
			virtual bool HandleEvent(SDL_Event &event) = 0;
		};
	}
}
#endif