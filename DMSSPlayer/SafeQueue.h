#ifndef __DMSS_SAFEQUEUE_H
#define __DMSS_SAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace dmss
{
	// �����̰߳�ȫqueue
	template <class T>
	class SafeQueue
	{
	private:
		// Ĭ�ϵȴ�ʱ�䣬��λ����
		static const int WAITTIME = 100;

	private:
		// C++ 11�̲߳���ȫqueue
		std::queue<T> m_queue;

		// �̻߳������
		mutable std::mutex m_mutex;

		// ��������
		std::condition_variable m_cVar;

	public:
		SafeQueue(){};
		~SafeQueue(){};

	public:
		// ����
		void enqueue(T &obj)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(std::forward<T>(obj));

			// ֪ͨ��������
			m_cVar.notify_one();
		};

		// ����
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