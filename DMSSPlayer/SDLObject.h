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
	// ����SDL�¼�������false���������¼�����֮�������¼�
	virtual bool HandleEvent(SDL_Event &event)=0;

	// ��Ӽ�������
	virtual void AddListener(SDLListener *listener);

	// ȥ����������
	virtual void RemoveListener(SDLListener *listener);

	// ��ռ�������
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