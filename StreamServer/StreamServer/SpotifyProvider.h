#pragma once
#include "ThreadSafeQueue.h"
#include "AudioChunk.h"
class SpotifyProvider
{
public:
	void StartThread();
	SpotifyProvider(const char* username, const char* password, const char* listname, ThreadSafeQueue<AudioChunk>* queue);
	~SpotifyProvider();
private:
	void Login();
	static void ProcessThread();
	const char * m_username;
	const char * m_password;
};

