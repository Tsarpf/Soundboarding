#pragma once

#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#define BUFSIZE 512
class Server
{
	//variables
	LPTSTR lpszPipename;
	std::mutex mutex;
	std::condition_variable condVar;
	bool newData;


	//functions
	void StreamDataToClient(HANDLE);
	void GetData();

	//constructors and overloads
	Server(const Server &);
	Server& operator=(const Server &);
public:
	Server(std::string);
	~Server();
	int Run();
};