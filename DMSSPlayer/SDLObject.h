#ifndef __DMSS_OBJECT_H
#define __DMSS_OBJECT_H

#include "SDLCommon.h"
#include <string>
#include <set>

class SDLListener
{
public:
	SDLListener(){};
	virtual ~SDLListener(){};

public:
	virtual void Update(void *data)=0;
};

class SDLObject
{
public:
	SDLObject();
	virtual ~SDLObject();

private:
	std::set<SDLListener *> m_listeners;

public:
	// 处理SDL事件，返回false继续传递事件，反之处理完事件
	virtual bool HandleEvent(SDL_Event &event)=0;

	// 添加监听对象
	virtual void AddListener(SDLListener *listener);

	// 去除监听对象
	virtual void RemoveListener(SDLListener *listener);

	// 清空监听对象
	virtual void ClearListener();

protected:
	virtual void Update(void *data);

private:
	bool Find(SDLListener *listener);
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