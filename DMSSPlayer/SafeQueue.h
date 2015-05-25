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
		void enqueue(T obj)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(obj);
			//fprintf(stdout, "enqueueSize = %d\n", m_queue.size());
		};

		// ����
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