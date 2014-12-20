#pragma once
class SpotifyProvider
{
public:
	void StartThread();
	SpotifyProvider(const char* username, const char* password);
	~SpotifyProvider();
private:
	void Login();
	const char * m_username;
	const char * m_password;



};

