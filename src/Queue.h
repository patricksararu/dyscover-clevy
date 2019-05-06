//---------------------------------------------------------------------------

#ifndef QueueH
#define QueueH

#include <mutex>
#include <queue>
#include <condition_variable>

//---------------------------------------------------------------------------

template<typename T>
class Queue
{
public:
	void Enqueue(const T& value)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.push_back(value);
		}

		m_condition.notify_all();
	}

	T Dequeue()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while (m_queue.empty()) { m_condition.wait(lock); }
		T value(std::move(m_queue.front()));
		m_queue.pop_front();
		return value;
	}

	bool TryDequeue(T& value)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_queue.empty()) { return false; }
		value = std::move(m_queue.front());
		m_queue.pop_front();
		return true;
	}

private:
	std::mutex m_mutex;
	std::deque<T> m_queue;
	std::condition_variable m_condition;
};

#endif
