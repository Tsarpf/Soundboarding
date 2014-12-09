#include "Server.h"
#include <string>
#include <vector>
#include "WorkaroundClient.h"

#pragma warning(disable:4996)


Server::Server() : fConnected(false), dwThreadId(0), hPipe(INVALID_HANDLE_VALUE), hThread(NULL), lpszPipename(TEXT("\\\\.\\pipe\\mynamedpipe"))
{
}


Server::~Server()
{
}

int Server::Run()
{
	DWORD waThreadId = 0;
	CreateThread(NULL, 0, &Server::WorkaroundThread, (LPVOID)this, 0, &waThreadId);

	for (;;)
	{
		_tprintf(TEXT("\nPipe Server: Main thread awaiting client connection on %s\n"), lpszPipename);
		hPipe = CreateNamedPipe(
			lpszPipename,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			100,                        // client time-out 
			NULL);                    // default security attribute 

		if (hPipe == INVALID_HANDLE_VALUE)
		{
			_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
			return -1;
		}

		// Wait for the client to connect; if it succeeds, 
		// the function returns a nonzero value. If the function
		// returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 


		fConnected = ConnectNamedPipe(hPipe, NULL) ?
		TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (fConnected)
		{
			printf("Client connected, creating a processing thread.\n");

			// Create a thread for this client. 
			hThread = CreateThread(
				NULL,              // no security attribute 
				0,                 // default stack size 
				&Server::InstanceThread,    // thread proc
				(LPVOID)this,    // thread parameter 
				//(LPVOID)hPipe,    // thread parameter 
				0,                 // not suspended 
				&dwThreadId);      // returns thread ID 

			if (hThread == NULL)
			{
				_tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());
				return -1;
			}
			else CloseHandle(hThread);
		}
		else
		{
			// The client could not connect, so close the pipe. 
			CloseHandle(hPipe);
		}
	}

	return 0;
}

DWORD WINAPI Server::WorkaroundThread(LPVOID that)
{
	Sleep(1000);
	WorkaroundClient* client = new WorkaroundClient();
	client->Run();
	return 1;
}


HANDLE Server::GetPipe()
{
	return hPipe;
}

//DWORD WINAPI Server::InstanceThread(LPVOID lpvParam)
DWORD WINAPI Server::InstanceThread(LPVOID that)

// This routine is a thread processing function to read from and reply to a client
// via the open pipe connection passed from the main loop. Note this allows
// the main loop to continue executing, potentially creating more threads of
// of this procedure to run concurrently, depending on the number of incoming
// client connections.
{
	HANDLE hHeap = GetProcessHeap();
	//TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
	//TCHAR* pchReply = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
	//byte* pchReply = (byte*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(byte));
	//byte* pchRequest = (byte*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(byte)); 
	byte* pchReply = new byte[BUFSIZE];
	byte* pchRequest = new byte[BUFSIZE];

	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
	BOOL fSuccess = FALSE;
	HANDLE hPipe = NULL;

	// Do some extra error checking since the app will keep running even if this
	// thread fails.

	//if (lpvParam == NULL)
	if (that == NULL)
	{
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
		printf("   InstanceThread exitting.\n");
		if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
		//if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}


	if (pchRequest == NULL)
	{
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL heap allocation.\n");
		printf("   InstanceThread exitting.\n");
		if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
		return (DWORD)-1;
	}

	if (pchReply == NULL)
	{
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL heap allocation.\n");
		printf("   InstanceThread exitting.\n");
		if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}

	// Print verbose messages. In production code, this should be for debugging only.
	printf("InstanceThread created, receiving and processing messages.\n");

	// The thread's parameter is a handle to a pipe object instance. 

	//Server* server = (Server*)that;
	hPipe = ((Server*)that)->GetPipe();
	
	//hPipe = (HANDLE)((Server*)that)->GetPipe();

	char* inputBuf = new char[BUFSIZE];
	for (int i = 0; i < BUFSIZE; i++)
	{
		inputBuf[i] = NULL;
	}
	int count = 0;
	// Loop until done reading
	while (1)
	{
		// Read client requests from the pipe. This simplistic code only allows messages
		// up to BUFSIZE characters in length.
		PeekNamedPipe(hPipe, inputBuf, BUFSIZE*sizeof(char), &cbBytesRead, NULL, NULL);
		if (cbBytesRead > 0)
		{
			fSuccess = ReadFile(
				hPipe,        // handle to pipe 
				inputBuf,    // buffer to receive data 
				BUFSIZE*sizeof(byte), // size of buffer 
				&cbBytesRead, // number of bytes read 
				NULL);        // not overlapped I/O 

			if (!fSuccess || cbBytesRead == 0)
			{
				if (GetLastError() == ERROR_BROKEN_PIPE)
				{
					_tprintf(TEXT("InstanceThread: client disconnected.\n"), GetLastError());
				}
				else
				{
					_tprintf(TEXT("InstanceThread ReadFile failed, GLE=%d.\n"), GetLastError());
				}
				break;
			}

			printf("got message %s", inputBuf);
			HeapFree(hHeap, 0, inputBuf);
		}
		else
		{

		}

		// Process the incoming message.
		((Server*)that)->GetAnswerToRequest((byte*)inputBuf, &pchReply, &cbReplyBytes, count);
		// Write the reply to the pipe. 
		Sleep(100);
		//FlushFileBuffers(hPipe);
		fSuccess = WriteFile(
			hPipe,        // handle to pipe 
			pchReply,     // buffer to write from 
			cbReplyBytes, // number of bytes to write 
			&cbWritten,   // number of bytes written 
			NULL);        // not overlapped I/O 

		printf("%d written\n", count);
		count++;

		if (!fSuccess || cbReplyBytes != cbWritten)
		{
			_tprintf(TEXT("InstanceThread WriteFile failed, GLE=%d.\n"), GetLastError());
			break;
		}


		//FlushFileBuffers(hPipe);
		Sleep(1000);
		//delete pchReply;
	}

	// Flush the pipe to allow the client to read the pipe's contents 
	// before disconnecting. Then disconnect the pipe, and close the 
	// handle to this pipe instance. 

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	delete[] inputBuf;
	delete[] pchReply;
	//HeapFree(hHeap, 0, pchReply);

	printf("InstanceThread exitting.\n");
	return 1;
}

VOID Server::GetAnswerToRequest(byte pchRequest[], byte* pchReply[], LPDWORD pchBytes, int count)
{
	if (pchRequest[0] != NULL)
	{
		_tprintf(TEXT("Client Request String:\"%s\"\n"), pchRequest);
	}

	std::string str = "Answer number ";
	str += std::to_string(count);

	char* c_str = new char[str.length()+1];
	memcpy(c_str, str.c_str(), str.length() + 1);

	*pchReply = (byte*)c_str;

	//pchReply = param;

	//*pchBytes = (lstrlen(pchReply) + 1)*sizeof(TCHAR);
	*pchBytes = sizeof(char) * (str.length() + 1);
}