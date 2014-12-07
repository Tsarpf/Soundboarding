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
	VOID GetAnswerToRequest(byte pchRequest[], byte* pchReply[], LPDWORD pchBytes, int count);

	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

	Server(const Server &);
	Server& operator=(const Server &);

	static DWORD WINAPI Server::WorkaroundThread(LPVOID that);

public:
	HANDLE GetPipe();
	Server();
	~Server();
	int Run();
};