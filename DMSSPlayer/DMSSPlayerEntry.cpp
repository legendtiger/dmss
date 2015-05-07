// DMSSPlayer.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "SDLWindow.h"
#include "SDLPicture.h"
#include "SDLButton.h"
#include "SDLVideoWnd.h"
#include "FFCommon.h"
#include "SDLApplication.h"
#include "FFAVDecoder.h"

int main(int argc, char* argv[])
{
	SDLWindow window("DMSS media player");
	SDLVideoWnd player(window);
	SDLButton bt(window, "", "k:/javastudy/button.bmp");
	FFAVDecoder avDecoder(player);
	if (avDecoder.Play("k:/javastudy/test.avi") == 0)
	{
		SDLApplication app(&window);
		app.Execute();
	}	
	return 0;
}

