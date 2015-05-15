#include "Object.h"
using namespace dmss;

	Object::Object()
	{

	}

	Object::~Object()
	{

	}

	void Object::Destroy()
	{

	}

	Observable::Observable()
	{

	}
	Observable::~Observable()
	{

	}
	bool Observable::Find(Observer *listener)
	{
		return this->m_listeners.find(listener) != this->m_listeners.end();
	}

	// 添加监听对象
	void Observable::AddListener(Observer *listener)
	{
		if (!Find(listener))
		{
			this->m_listeners.insert(listener);
		}
	}

	// 去除监听对象
	void Observable::RemoveListener(Observer *listener)
	{
		this->m_listeners.erase(listener);
	}

	// 清除所有监听对象
	void Observable::ClearListener()
	{
		this->m_listeners.clear();
	}

	void Observable::Update(void *data)
	{
		std::set <Observer *>::iterator it = m_listeners.begin();
		while (it != m_listeners.end())
		{
			(*it)->Update(data);
		}
	}
