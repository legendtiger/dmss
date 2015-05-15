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

			// 解码线程退出/完成
			bool m_decodeFinished;

			// 结束播放
			bool m_stopFlag;

		public:
			Video(IWindow* parent, int x = 0, int y = 0, int w = 800, int h = 600);
			~Video();

		public:
			// 更新帧数据
			void UpdateFrame(void * pixels, int pitch);

			// 初始化视频
			void InitVideo(int w, int h, Uint32 format = SDL_PIXELFORMAT_YV12);

			bool HandleEvent(SDL_Event &event);

			void Flip();

			// 停止播放
			bool CanStop();

			// 解码线程完成或终止
			void DecodeFinished();

		protected:
			SDL_Texture * DisplayTexture();

			bool Changed();

			void Clean();
		public:
			// 初始化音频
			void InitAudio(int freq, int channels, int samples = SDL_AUDIO_BUFFER_SIZE);
		};
	}
}
#endif