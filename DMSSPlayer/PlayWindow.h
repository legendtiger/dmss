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
			// SDL����
			SDL_Window *m_pWindow = NULL;

			// SDL Renderer
			SDL_Renderer *m_pRenderer = NULL;

			// ���ڱ���
			std::string m_title;

			// ���ڱ�־
			Uint32 m_flags;

			// �Ƿ�ˢ�´���
			bool m_isChanged = true;

			// ������ʾ����
			std::vector <Component*> m_items;

			VideoRender *m_pVideoRender = NULL;
			AVPlayer *m_player = NULL;
			Unpacker *m_pUnpacker = NULL;

		public:
			// ��ȡ��ʾ����
			SDL_Rect GetRenderRect();

			// ��ȡSDL����
			SDL_Window* GetWindow();

			// ��ȡRenderer
			SDL_Renderer* GetRenderer();

			// ˢ�´���
			void Flip();

			// �������ڳߴ�
			void Resize(int w, int h);

			// �ƶ�����
			void Move(int x, int y);

			// ����ӿؼ�
			bool Add(Component* child);

			// ����SDL�¼�������false���������¼�����֮�������¼�
			bool HandleEvent(SDL_Event &event);

		private:
			// �Ӵ����Ƿ�ı䣬��Ҫ�ػ�
			bool ItemIsChanged();
		public:
			void Play(std::string filename);
		};
	}
}

#endif