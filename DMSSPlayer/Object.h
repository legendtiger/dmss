#ifndef __DMSS_OBJECT_H
#define __DMSS_OBJECT_H

#include "ImportSDL.h"
#include <string>
#include <set>

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

	class Observable:public Object
	{
	public:
		Observable();
		virtual ~Observable();

	private:
		std::set<Observer *> m_listeners;

	public:
		// 处理SDL事件，返回false继续传递事件，反之处理完事件
		virtual bool HandleEvent(SDL_Event &event) = 0;

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