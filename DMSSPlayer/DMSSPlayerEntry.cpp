// DMSSPlayer.cpp : 定义控制台应用程序的入口点。
//
#include "SDLUtil.h"
#include "SDLWindow.h"

int main(int argc, char* argv[])
{
	SDLUtil::InitSDL();
	SDLWindow window("DMSS media player");
	window.show();
	SDLUtil::CloseSDL();
	return 0;
}

