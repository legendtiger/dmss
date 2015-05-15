#ifndef __DMSS_VIDEO_WND_H
#define __DMSS_VIDEO_WND_H

#include "Component.h"
#include "IWindow.h"
namespace dmss
{
	namespace sdl
	{
		class Video : public Component
		{
		private:
			static const int SDL_AUDIO_BUFFER_SIZE = 1024;

			SDL_Texture *m_pVTexture = NULL;

			bool m_isChanged = false;

			// �����߳��˳�/���
			bool m_decodeFinished;

			// ��������
			bool m_stopFlag;

		public:
			Video(IWindow* parent, int x = 0, int y = 0, int w = 800, int h = 600);
			~Video();

		public:
			// ����֡����
			void UpdateFrame(void * pixels, int pitch);

			// ��ʼ����Ƶ
			void InitVideo(int w, int h, Uint32 format = SDL_PIXELFORMAT_YV12);

			bool HandleEvent(SDL_Event &event);

			void Flip();

			// ֹͣ����
			bool CanStop();

			// �����߳���ɻ���ֹ
			void DecodeFinished();

		protected:
			SDL_Texture * DisplayTexture();

			bool Changed();

			void Clean();
		public:
			// ��ʼ����Ƶ
			void InitAudio(int freq, int channels, int samples = SDL_AUDIO_BUFFER_SIZE);
		};
	}
}
#endif