#pragma once

#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "ThreadSafeQueue.h"
#include "AudioChunk.h"

#define BUFSIZE 512


class Server
{
	//variables
	LPTSTR lpszPipename;
	std::mutex mutex;
	std::condition_variable condVar;
	bool newData;
	ThreadSafeQueue<AudioChunk>* m_queue;
	int m_port;


	//functions
	void GetData();

	//constructors and overloads
	Server(const Server &);
	Server& operator=(const Server &);
public:
	Server(int port, ThreadSafeQueue<AudioChunk>* queue);
	~Server();
	void Run();
};
