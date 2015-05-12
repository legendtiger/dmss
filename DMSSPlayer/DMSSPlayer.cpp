#include "DMSSPlayer.h"
#include "SDLWindow.h"
#include "SDLPicture.h"
#include "SDLButton.h"
#include "SDLVideoWnd.h"
#include "FFCommon.h"
#include "SDLApplication.h"
#include "FFAVDecoder.h"

DMSSPlayer::DMSSPlayer(std::string title, int w, int h, Uint32 flags)
:SDLWindow(title, w, h, flags)
{
}


DMSSPlayer::~DMSSPlayer()
{
}


void DMSSPlayer::Show()
{
	/*
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
	//*/
	SDLWindow::show();
	m_pToolBar->show();
}

void DMSSPlayer::Create()
{
	SDLWindow::Create(this->Width(), this->Height());

	// 创建toolbar窗口
	m_pToolBar = new SDLWindow(400, 40, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	// 创建按钮
	SDLButton bt(*m_pToolBar, "", "K:/JavaStudy/button.bmp");
}