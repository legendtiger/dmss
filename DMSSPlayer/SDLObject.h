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
	// 处理SDL事件，返回false继续传递事件，反之处理完事件
	virtual bool HandleEvent(SDL_Event &event){ return false; };

	// 克隆对象
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