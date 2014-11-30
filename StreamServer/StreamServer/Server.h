#pragma once

#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>

#define BUFSIZE 512
class Server
{
	BOOL   fConnected = FALSE;
	DWORD  dwThreadId = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;

	static DWORD WINAPI InstanceThread(LPVOID);
	VOID GetAnswerToRequest(LPTSTR pchRequest, LPTSTR pchReply, LPDWORD pchBytes);

	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

	Server(const Server &);
public:
	HANDLE GetPipe();
	Server();
	~Server();
	int Run();
};

