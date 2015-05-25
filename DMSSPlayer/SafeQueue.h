#ifndef __DMSS_SAFEQUEUE_H
#define __DMSS_SAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace dmss
{
	// 定义线程安全queue
	template <class T>
	class SafeQueue
	{
	private:
		// 默认等待时间，单位毫秒
		static const int WAITTIME = 100;

	private:
		// C++ 11线程不安全queue
		std::queue<T> m_queue;

		// 线程互斥对象
		mutable std::mutex m_mutex;

		// 条件变量
		std::condition_variable m_cVar;

	public:
		SafeQueue(){};
		~SafeQueue(){};

	public:
		// 进队
		void enqueue(T obj)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(obj);
			//fprintf(stdout, "enqueueSize = %d\n", m_queue.size());
		};

		// 出队
		T dequeue()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			//fprintf(stdout, "dequeueSize = %d\n", m_queue.size());
			if (m_queue.empty())
			{
				return NULL;
			}

			T ret = m_queue.front();
			m_queue.pop();
			return ret;
		};
	};
}
#endif