#include <strsafe.h>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename Data>
class ThreadSafeQueue 
{
private:
	std::queue<Data> queue;
	std::condition_variable conditionVariable;
	mutable std::mutex mutex;
public:

	std::condition_variable getConditionVariable()
	{
		return conditionVariable;
	}

	void push(const Data& data)
	{
		std::unique_lock < std::mutex >> lock(mutex);

		queue.push(data);

		conditionVariable.notify_one();
	}

	bool empty() const
	{
		std::unique_lock < std::mutex >> lock(mutex);
		return queue.empty();
	}

	Data& front()
	{
		std::unique_lock < std::mutex >> lock(mutex);
		return queue.front();
	}

	Data const& front() const
	{
		std::unique_lock < std::mutex >> lock(mutex);
		return queue.front();
	}

	void pop()
	{
		std::unique_lock < std::mutex >> lock(mutex);
		queue.pop();
	}
};
