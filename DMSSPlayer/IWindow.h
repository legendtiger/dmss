#ifndef __DMSS_WINDOWINTERFACE_H
#define __DMSS_WINDOWINTERFACE_H

#include "Object.h"
#include "SDLItemBase.h"

namespace dmss
{
	class IWindow : public Object
	{
	public:
		IWindow(){};
		virtual ~IWindow(){};

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
		virtual bool Add(SDLItemBase* child) = 0;

		// 处理SDL事件，返回false继续传递事件，反之处理完事件
		virtual bool HandleEvent(SDL_Event &event) = 0;
	};
}
#endif