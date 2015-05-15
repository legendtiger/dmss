#ifndef __DMSS_WINDOW_H
#define __DMSS_WINDOW_H

#include "ImportSDL.h"
#include "IWindow.h"
#include "Component.h"

#include <string>
#include <vector>
namespace dmss
{
	namespace sdl
	{
		class SDLWindow : public IWindow
		{
		public:
			static const int DEFAULT_WIDTH = 1024;
			static const int DEFAULT_HIGHT = 768;
			static const int DEFAULT_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

		public:
			SDLWindow(std::string title, int w = DEFAULT_WIDTH, int h = DEFAULT_HIGHT, Uint32 flags = DEFAULT_FLAGS);
			SDLWindow(int w = DEFAULT_WIDTH, int h = DEFAULT_HIGHT, Uint32 flags = DEFAULT_FLAGS);
			~SDLWindow();

		private:
			// SDL窗口
			SDL_Window *m_pWindow = NULL;

			// SDL Renderer
			SDL_Renderer *m_pRenderer = NULL;

			// 是否刷新窗口
			bool m_isChanged = true;

			float m_scaleX = 1.0;
			float m_scaleY = 1.0;

			// 窗口标题
			std::string m_title;

			// 窗口尺寸
			int m_width;
			int m_height;

			// 窗口标志
			Uint32 m_flags;

			// 保存显示对象
			std::vector <Component*> m_items;
		public:
			// 获取显示区域
			SDL_Rect GetRenderRect();

			// 获取SDL窗口
			SDL_Window* GetWindow();

			// 获取Renderer
			SDL_Renderer* GetRenderer();

			// 调整窗口尺寸
			void Resize(int w, int h);

			// 移动窗口
			void Move(int x, int y);

			// 继承SDLObject的HandleEvent方法
			bool HandleEvent(SDL_Event &event);

			// 刷新窗口
			void Flip();

			// 通知窗口释放资源
			void Destroy();

			virtual void show();

			float GetScaleX();
			float GetScaleY();

			int Width();
			int Height();

			// 添加显示对象
			bool Add(Component *item);

		protected:
			virtual void Create(int w, int h);

		private:
			// 子窗口是否改变，需要重绘
			bool ItemIsChanged();
		};
	}
}
#endif