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
		// �ͷŶ�����Դ
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
		// ��Ӽ�������
		virtual void AddListener(Observer *listener);

		// ȥ����������
		virtual void RemoveListener(Observer *listener);

		// ��ռ�������
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