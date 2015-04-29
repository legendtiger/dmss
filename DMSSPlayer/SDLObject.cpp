#include "SDLObject.h"

SDLObject::SDLObject()
{

}

SDLObject::~SDLObject()
{

}

bool SDLObject::Find(SDLListener *listener)
{
	return this->m_listeners.find(listener) != this->m_listeners.end();
}

// ��Ӽ�������
void SDLObject::AddListener(SDLListener *listener)
{
	if (!Find(listener))
	{
		this->m_listeners.insert(listener);
	}
}

// ȥ����������
void SDLObject::RemoveListener(SDLListener *listener)
{
	this->m_listeners.erase(listener);
}

// ������м�������
void SDLObject::ClearListener()
{	
	this->m_listeners.clear();
}

void SDLObject::Update(void *data)
{
	std::set <SDLListener *>::iterator it = m_listeners.begin();
	while (it != m_listeners.end())
	{
		(*it)->Update(data);
	}
}