#pragma once

#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>

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
	Server();
	~Server();
	int Run();
};