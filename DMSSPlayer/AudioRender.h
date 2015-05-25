#ifndef __DMSS_AUDIORENDER_H
#define __DMSS_AUDIORENDER_H

#include "Object.h"
namespace dmss
{
	namespace sdl
	{
		class AudioRender : public Observer
		{
		public:
			AudioRender();
			~AudioRender();

		public:
			static const int DEFAULTFREQUENCY = 1024;

		public:
			// ��������
			void Update(void *data, int size);

			// ��ʼ����Ƶ����
			void InitAudio(int freq, int channels, int samples);
		public:
			void Pause();
			void Stop();
			void Play();
		};
	}
}

#endif