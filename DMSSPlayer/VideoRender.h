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
			// ��ͼ����
			SDL_Texture *m_pVTexture = NULL;

			// �Ƿ����������
			bool m_isChanged = false;

		public:
			bool Changed();

			// ����SDL�¼�������false���������¼�����֮�������¼�
			bool HandleEvent(SDL_Event &event);

			// ��������
			void Update(void *data, int size);

			// ��ʼ����ͼ
			void InitTexture(int w, int h, Uint32 format = SDL_PIXELFORMAT_YV12);

			void Flip();

		protected:
			// �ؼ���ͼ����
			SDL_Texture * DisplayTexture();
		};
	}
}

#endif