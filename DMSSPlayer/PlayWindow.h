#ifndef __DMSS_PLAYWINDOW_H
#define __DMSS_PLAYWINDOW_H

#include "IWindow.h"
#include <vector>
#include "VideoRender.h"
#include "AVPlayer.h"
#include "Unpacker.h"

using namespace dmss::ffmpeg;
namespace dmss
{
	namespace sdl
	{
		class PlayWindow : public IWindow
		{
		public:
			PlayWindow(std::string title, int w = DEFAULT_WIDTH, int h = DEFAULT_HIGHT, Uint32 flags = DEFAULT_FLAGS);
			~PlayWindow();

		public:
			static const int DEFAULT_WIDTH = 1024;
			static const int DEFAULT_HIGHT = 768;
			static const int DEFAULT_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
			//static const int DEFAULT_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

		private:
			// SDL窗口
			SDL_Window *m_pWindow = NULL;

			// SDL Renderer
			SDL_Renderer *m_pRenderer = NULL;

			// 窗口标题
			std::string m_title;

			// 窗口标志
			Uint32 m_flags;

			// 是否刷新窗口
			bool m_isChanged = true;

			// 保存显示对象
			std::vector <Component*> m_items;

			VideoRender *m_pVideoRender = NULL;
			AVPlayer *m_player = NULL;
			Unpacker *m_pUnpacker = NULL;

		public:
			// 获取显示区域
			SDL_Rect GetRenderRect();

			// 获取SDL窗口
			SDL_Window* GetWindow();

			// 获取Renderer
			SDL_Renderer* GetRenderer();

			// 刷新窗口
			void Flip();

			// 调整窗口尺寸
			void Resize(int w, int h);

			// 移动窗口
			void Move(int x, int y);

			// 添加子控件
			bool Add(Component* child);

			// 处理SDL事件，返回false继续传递事件，反之处理完事件
			bool HandleEvent(SDL_Event &event);

		private:
			// 子窗口是否改变，需要重绘
			bool ItemIsChanged();
		public:
			void Play(std::string filename);
		};
	}
}

#endif