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

	// ��Ӽ�������
	void Observable::AddListener(Observer *listener)
	{
		if (!Find(listener))
		{
			this->m_listeners.insert(listener);
		}
	}

	// ȥ����������
	void Observable::RemoveListener(Observer *listener)
	{
		this->m_listeners.erase(listener);
	}

	// ������м�������
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
