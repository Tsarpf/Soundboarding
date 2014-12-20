#pragma once
class SpotifyProvider
{
public:
	void Login(const char* username, const char* password);
	void ProcessEvents();
	static void StartThread();
	SpotifyProvider();
	~SpotifyProvider();
private:



};

