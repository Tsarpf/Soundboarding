#pragma once
#include "AllowWindowsPlatformTypes.h"
#include <strsafe.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <algorithm>
#include "HideWindowsPlatformTypes.h"

//template<typename Data>
class ThreadSafeQueue 
{
private:
	//std::queue<Data> queue;
	//std::condition_variable conditionVariable;
	mutable std::mutex mutex;

	char* data = 0;
	int bytesLeft = 0;

public:

	void enqueue(const char* newData, const int byteCount)
	{
		std::unique_lock < std::mutex > lock(mutex);
		int newSize = byteCount + bytesLeft;
		char * newDataStore = (char*)realloc(data, newSize);
		memcpy(newDataStore + bytesLeft, newData, byteCount);
		//remember to free newData in the calling function
		bytesLeft += byteCount;
		data = newDataStore;
	}

	bool empty() const
	{
		std::unique_lock < std::mutex > lock(mutex);
		return bytesLeft == 0;
		//return queue.empty();
	}

	/*
	void waitForData()
	{
		if (!empty())
			return;


		std::unique_lock < std::mutex > lock(mutex);
		conditionVariable.wait(lock, [this] { return bytesLeft != 0; });
	}
	*/

	int dequeue(int byteCount, char** chunk)
	{
		std::unique_lock < std::mutex > lock(mutex);
		if (byteCount == 0 || bytesLeft == 0)
		{
			return 0;
		}

		int bytesToCopy = std::min(bytesLeft, byteCount);
		*chunk = new char[bytesToCopy];

		memcpy(*chunk, data, bytesToCopy);

		char* newPos = data + bytesToCopy;
		bytesLeft -= bytesToCopy;

		memcpy(data, newPos, bytesLeft);

		data = (char*)realloc(data, bytesLeft);

		return bytesToCopy;
	}
};
