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
			// ��ʾλ�ô�С
			SDL_Rect m_rt;

			// ��Ⱦ��
			IWindow *m_pParent;

		public:
			virtual ~Component();
			Component(IWindow* parent, int x = 0, int y = 0, int w = 0, int h = 0);

			// ����itemλ��
			void SetPosition(int x, int y);

			// ����item��С
			void SetSize(int w, int h);

			// ˢ��item
			virtual void Flip();

			// ��ɫ
			virtual void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag = SDL_TRUE);

			// ��ȡitemλ�ô�С
			SDL_Rect GetRect();

			virtual bool PointInItem(int x, int y);

			// ��ȡ������
			IWindow *GetParent();

			virtual bool Changed() = 0;

			// ����SDL�¼�������false���������¼�����֮�������¼�
			virtual bool HandleEvent(SDL_Event &event) = 0;

		protected:

			// �ؼ���ͼ����
			virtual SDL_Texture * DisplayTexture() = 0;
		};
	}
}
#endif