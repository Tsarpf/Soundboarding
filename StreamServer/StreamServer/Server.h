#pragma once

#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "ThreadSafeQueue.h"

#define BUFSIZE 512

struct AudioChunk
{
	int sampleRate;
	int channels;
	int frameCount;
	void* frames;
};

class Server
{
	//variables
	LPTSTR lpszPipename;
	std::mutex mutex;
	std::condition_variable condVar;
	bool newData;
	ThreadSafeQueue<AudioChunk>* audioQueue;


	//functions
	void StreamDataToClient(HANDLE);
	void GetData();

	//constructors and overloads
	Server(const Server &);
	Server& operator=(const Server &);
public:
	Server(std::string, ThreadSafeQueue<AudioChunk> *);
	~Server();
	int Run();
};
