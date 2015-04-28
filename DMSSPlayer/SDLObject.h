#ifndef __DMSS_OBJECT_H
#define __DMSS_OBJECT_H

#include "SDLCommon.h"
#include <string>

class SDLObject
{
public:
	SDLObject(){};
	virtual ~SDLObject(){};

public:
	// ����SDL�¼�������false���������¼�����֮�������¼�
	virtual bool HandleEvent(SDL_Event &event){ return false; };

	// ��¡����
	virtual SDLObject & Clone(){ return SDLObject(); };
};

class SDLError
{
private:
	std::string m_msg = "";

public:
	SDLError(){ this->m_msg = SDL_GetError(); };
	SDLError(std::string error){ this->m_msg = error; };
	~SDLError(){};

public:
	std::string GetError(){ return this->m_msg; };
};
#endif