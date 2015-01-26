// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundBoarding.h"
#include "MySoundWaveStreaming.h"

#include "AllowWindowsPlatformTypes.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <strsafe.h>
#include <thread>

#include "HideWindowsPlatformTypes.h"

#include "ThreadSafeQueue.h"

//DECLARE_CYCLE_STAT(TEXT("Sound Mod Generate Data"), STAT_SoundModGeneratePCMData, STATGROUP_Audio);

UMySoundWaveStreaming::UMySoundWaveStreaming(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP), Time(0.0f), Frequency(100.0f), m_queue()
{
	SampleRate = 44100;
	NumChannels = 2;
	Duration = INDEFINITELY_LOOPING_DURATION;
	bProcedural = true;
	bLooping = false;
	DeltaTime = 1.0f / SampleRate;

	std::thread t(StreamClient, &m_queue);
	t.detach();
}

int32 UMySoundWaveStreaming::GeneratePCMData(uint8* PCMData, const int32 SamplesNeeded)
{
	/*
	float TimeStart = Time;

	Omega = 2.0f * PI * Frequency;	// angular frequency [rad/s]

	for (int i = 0; i < SamplesNeeded; i++)
	{
		Time = TimeStart + i * DeltaTime;
		int32 a = FMath::RoundToInt(65535.0f * (1.0f + FMath::Sin(Omega*Time)) * 0.5f);
		if (a > 65535)
		{
			a = 65535;
		}
		else if (a < 0)
		{
			a = 0;
		}
		uint16 ua = (uint16)a;
		PCMData[2 * i] = ua;			// Low Byte
		PCMData[2 * i + 1] = ua >> 8;	// High Byte
	}

	//Time = Time + SamplesNeeded * DeltaTime; // INCORRECT LINE
	Time = TimeStart + SamplesNeeded * DeltaTime; // CORRECTED LINE

	int32 BytesProvided = SamplesNeeded * 2;
	return BytesProvided;
	*/
	int byteCount = SamplesNeeded * sizeof(int16);
	char* data = 0;

	int dataRetrieved = m_queue.dequeue(byteCount, &data);

	memcpy((void*)PCMData, data, byteCount);
	delete[] data;
	return dataRetrieved;
}

void UMySoundWaveStreaming::StreamClient(ThreadSafeQueue* queue)
{
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

#define DEFAULT_PORT "3666"

	// Resolve the server address and port
	auto iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}



	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}


	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}


#define DEFAULT_BUFLEN 512

	int recvbuflen = DEFAULT_BUFLEN;

	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];

	/*
	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}
	*/
	//printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	// Receive data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());

		queue->enqueue(recvbuf, iResult);

		//int derp = (int)recvbuf[0];
		//derp++;
		//int herp= (int)recvbuf[iResult / 2];
		//herp++;
		//int perp = (int)recvbuf[iResult - 1];
		//perp++;
		//printf("ses: %d %d %d \n", derp, herp, perp);
	} while (iResult > 0);
}