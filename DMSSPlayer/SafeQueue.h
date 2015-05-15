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
		void enqueue(T &obj)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(std::forward<T>(obj));

			// 通知已有数据
			m_cVar.notify_one();
		};

		// 出队
		T* dequeue(int timeout = COOPERATIVE_TIMEOUT_INFINITE)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_queue.empty())
			{
				if (m_cVar.wait_for(lock, timeout) == std::cv_status::timeout)
				{
					return NULL;
				}
			}
			T *ret = &m_queue.front();
			m_queue.pop();
			return ret;
		};
	};
}
#endif