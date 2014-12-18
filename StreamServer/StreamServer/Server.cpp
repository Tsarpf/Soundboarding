#include "Server.h"
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "ThreadSafeQueue.h"

#pragma warning(disable:4996)


Server::Server(std::string pipename, ThreadSafeQueue<AudioChunk> queue) : newData(false)
{
	int size = pipename.length() + 1;
	const char* text = new char[size];
	text = pipename.c_str();

	wchar_t* wtext = new wchar_t[size * 2];

	mbstowcs(wtext, text, size);//Plus null

	lpszPipename = wtext;






}


Server::~Server()
{
}

void Server::StreamDataToClient(HANDLE hPipe)
{
	
	unsigned long cbBytesRead = 0;
	unsigned long cbReplyBytes = 0;
	unsigned long cbWritten = 0;

	byte* sendMsg = new byte[BUFSIZE];
	byte* receiveMsg = new byte[BUFSIZE];

	char* inputBuf = new char[BUFSIZE];
	for (int i = 0; i < BUFSIZE; i++)
	{
		inputBuf[i] = NULL;
	}
	int count = 0;
	BOOL fSuccess = false;

	while (1)
	{
		PeekNamedPipe(hPipe, inputBuf, BUFSIZE*sizeof(char), &cbBytesRead, NULL, NULL);
		if (cbBytesRead > 0)
		{
			fSuccess = ReadFile(
				hPipe,        // handle to pipe 
				inputBuf,    // buffer to receive data 
				BUFSIZE*sizeof(byte), // size of buffer 
				&cbBytesRead, // number of bytes read 
				NULL);        // not async I/O 

			if (!fSuccess || cbBytesRead == 0)
			{
				if (GetLastError() == ERROR_BROKEN_PIPE)
				{
					_tprintf(TEXT("Client disconnected.\n"), GetLastError());
				}
				else
				{
					_tprintf(TEXT("ReadFile failed, GLE=%d.\n"), GetLastError());
				}
				break;
			}

			printf("got message %s", inputBuf);
		}

		//wait for data
		std::unique_lock<std::mutex> lock(mutex);
		condVar.wait(lock, [this] { return newData; });

		std::cout << "Got data";
		//get data from queue

		lock.unlock();
		//condVar.notify_one(); //This doesn't belong here, right?

		//form a packet out of data from queue

		//write data
		fSuccess = WriteFile(
			hPipe,        // handle to pipe 
			sendMsg,     // buffer to write from 
			cbReplyBytes, // number of bytes to write 
			&cbWritten,   // number of bytes written 
			NULL);        // not async I/O 

		printf("%d written\n", count);
		count++;

		if (!fSuccess || cbReplyBytes != cbWritten)
		{
			_tprintf(TEXT("InstanceThread WriteFile failed, GLE=%d.\n"), GetLastError());
			break;
		}
		Sleep(1000);
	}
}



/*

VOID Server::GetAnswerToRequest(byte pchRequest[], byte* pchReply[], LPDWORD pchBytes, int count)
	std::string str = "Answer number ";
	str += std::to_string(count);

	char* c_str = new char[str.length()+1];
	memcpy(c_str, str.c_str(), str.length() + 1);

	*pchReply = (byte*)c_str;

	//pchReply = param;

	//*pchBytes = (lstrlen(pchReply) + 1)*sizeof(TCHAR);
	*pchBytes = sizeof(char) * (str.length() + 1);
*/

int Server::Run()
{
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	bool fConnected = false;
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

		fConnected = ConnectNamedPipe(hPipe, NULL) ?
		TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (fConnected)
		{
			printf("Client connected, starting streaming etc.\n");
			StreamDataToClient(hPipe);
		}
		else
		{
			// The client could not connect, so close the pipe. 
			CloseHandle(hPipe);
		}
		Sleep(100);
	}

	return 0;
}