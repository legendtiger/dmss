// DMSSPlayer.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "SDLApplication.h"
#include "Decoder.h"
using namespace dmss::ffmpeg;
int main(int argc, char* argv[])
{
	Decoder decoder(NULL);
	decoder.Init("K:/JavaStudy/test.wmv");
	fprintf(stdout, decoder.DecodeInfo().Duration().c_str());
	fprintf(stdout, "type = %d\n", decoder.DecodeInfo().mediaType);
	//SDLApplication app;
	//app.Execute();
	getchar();
	return 0;
}

