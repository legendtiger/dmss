// DMSSPlayer.cpp : 定义控制台应用程序的入口点。
//
#include "SDLApplication.h"
#include "SDLVideoWnd.h"
#include "SDLButton.h"
#include "FFAVDecoder.h"

int main(int argc, char* argv[])
{
	SDLWindow window("DMSS media player");
	SDLVideoWnd player(window);
	SDLButton bt(window, "", "k:/javastudy/button.bmp");
	FFAVDecoder avDecoder(player);
	if (avDecoder.Init("k:/javastudy/test.avi"))
	{
		avDecoder.Play(0);
		SDLApplication app(&window);
		app.Execute();
	}
	return 0;
}

