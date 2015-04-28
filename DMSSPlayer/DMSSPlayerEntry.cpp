// DMSSPlayer.cpp : 定义控制台应用程序的入口点。
//
#include "SDLUtil.h"
#include "SDLWindow.h"
#include "SDLPicture.h"

int main(int argc, char* argv[])
{
	SDLUtil::InitSDL();
	//SDLWindow first("First window", 300, 100);
	//first.show();
	//first.Move(10, 10);
	SDLWindow window("DMSS media player");
	window.show();
	SDLPicture pic(window, "k:/javastudy/button.bmp", 0, 0);
	window.EventLoop();
	SDLUtil::CloseSDL();
	return 0;
}

