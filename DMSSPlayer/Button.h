#ifndef __DMSS_BUTTON_H
#define __DMSS_BUTTON_H

#include "Component.h"
#include <string>

namespace dmss
{
	namespace sdl
	{
		class Button : public Component
		{
		private:
			SDL_Texture *m_pTexture = NULL;
			SDL_Surface *m_pPicture = NULL;
			SDL_Texture *m_pText = NULL;

			// ��ť�ı�
			std::string m_text;

			// ��¼��ťnormal��focus״̬ͼƬλ��
			SDL_Rect m_rtNormal;
			SDL_Rect m_rtFocus;

			// ��¼����Ƿ��ڰ�ť��
			bool m_isMotion = false;

			bool m_isChange = false;

		public:
			~Button();
			Button(IWindow* parent, std::string text, std::string bkgFileName, int x = 0, int y = 0, int w = 80, int h = 35);

		public:
			void Flip();

			// ��ɫ
			void colorKey(Uint8 r, Uint8 g, Uint8 b, Uint32 flag = SDL_TRUE);

			// ����SDL�¼�������false���������¼�����֮�������¼�
			bool HandleEvent(SDL_Event &event);

			void Destroy();
		private:
			// ������ͼ
			void Update(void *data, int size);

		protected:
			// �Ƿ���Ҫ�ػ�
			bool Changed();

			// ȡ�ؼ���ͼ 
			SDL_Texture * DisplayTexture();
		};
	}
}
#endif
