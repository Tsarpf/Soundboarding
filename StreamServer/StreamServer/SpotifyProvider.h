#pragma once
class SpotifyProvider
{
public:
	void StartThread();
	SpotifyProvider(const char* username, const char* password, const char* listname);
	~SpotifyProvider();
private:
	void Login();
	static void ProcessThread();
	const char * m_username;
	const char * m_password;



};

