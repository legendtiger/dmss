// DMSSPlayer.cpp : �������̨Ӧ�ó������ڵ㡣
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

