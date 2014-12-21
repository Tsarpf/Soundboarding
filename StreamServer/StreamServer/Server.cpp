#include "Server.h"
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "ThreadSafeQueue.h"
//#include <WinSock2.h>
#include <stdint.h>
//#include <WS2tcpip.h>

#pragma warning(disable:4996)


Server::Server(int port, ThreadSafeQueue<AudioChunk>* queue)
{
	m_port = port;
	audioQueue = queue;

}


Server::~Server()
{
}


void Server::Run()
{
	sockaddr_in m_addr;
	memset(&m_addr, 0, sizeof(m_addr));

	int port = m_port;

	//create
	int m_sock = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "created socket" << std::endl;

	int on = 1;
	char * onOn = (char*)(&on);
	int err = setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, onOn, sizeof(onOn));

	std::cout << "setsockopt'd socket (err was " << err << ")" << std::endl;

	//bind

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons(port);

	//:: resolves scope, ie. forces to global namespace
	int bind_return = ::bind(m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));
	std::cout << "bind'd socket" << std::endl;


	//listen
	int maxConnections = 5;
	::listen(m_sock, maxConnections);
	std::cout << "listening" << std::endl;


	//accept
	int addr_length = sizeof(m_addr);


	//int maxLength = 500;
	//char buf[maxLength + 1];
	char* buf = new char[BUFSIZE];
	memset(buf, 0, BUFSIZE);


	while (true)
	{
		std::cout << "waiting to accept" << std::endl;
		int newSocket = accept(m_sock, (sockaddr *)&m_addr, &addr_length);
		std::cout << "accepted" << std::endl;

		while(true)
		{            
			try
			{
				int status = ::recv(newSocket, buf, BUFSIZE, 0);
				if (status == -1)
				{
					std::cout << "Error number: " << errno
						<< " when tried to receive." << std::endl;
				}
				else if (status == 0)
				{
					std::cout << "Status was 0 when tried to receive" << std::endl;
				}
				else
				{
					std::cout << "Received: '" << buf << "'." << std::endl;
				}

				//filenames were enumerated at the start of main.
				std::string data = "ses";
				::send(newSocket, data.c_str(), data.size(), 0);
				std::cout << "sent: '" << data << "'" << std::endl;
			}
			catch (std::string&)
			{
				std::cout << "Socket closed" << std::endl;
			}
			Sleep(1000);
		}
	}
}