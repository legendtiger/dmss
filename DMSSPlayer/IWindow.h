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
			IWindow(){};
			virtual ~IWindow(){};

		public:
			// ��ȡ��ʾ����
			virtual SDL_Rect GetRenderRect() = 0;

			// ��ȡSDL����
			virtual SDL_Window* GetWindow() = 0;

			// ��ȡRenderer
			virtual SDL_Renderer* GetRenderer() = 0;

			// ˢ�´���
			virtual void Flip() = 0;

			// �������ڳߴ�
			virtual void Resize(int w, int h) = 0;

			// �ƶ�����
			virtual void Move(int x, int y) = 0;

			// ����ӿؼ�
			virtual bool Add(Component* child) = 0;

			// ����SDL�¼�������false���������¼�����֮�������¼�
			virtual bool HandleEvent(SDL_Event &event) = 0;
		};
	}
}
#endif