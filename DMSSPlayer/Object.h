#ifndef __DMSS_OBJECT_H
#define __DMSS_OBJECT_H

#include <string>
#include <set>
namespace dmss
{
	class Observer
	{
	public:
		Observer(){};
		virtual ~Observer(){};

	public:
		virtual void Update(void *data) = 0;
	};

	class Object
	{
	public:
		Object();
		virtual ~Object();

	public:
		// 释放对象资源
		virtual void Destroy();
	};

	class Observable :public Object
	{
	public:
		Observable();
		virtual ~Observable();

	private:
		std::set<Observer *> m_listeners;

	public:
		// 添加监听对象
		virtual void AddListener(Observer *listener);

		// 去除监听对象
		virtual void RemoveListener(Observer *listener);

		// 清空监听对象
		virtual void ClearListener();

	protected:
		virtual void Update(void *data);

	private:
		bool Find(Observer *listener);
	};

	class Error
	{
	private:
		std::string m_msg = "";

	public:
		Error(){ };
		Error(std::string error){ this->m_msg = error; };
		~Error(){};

	public:
		std::string Message(){ return this->m_msg; };
	};
}
#endif