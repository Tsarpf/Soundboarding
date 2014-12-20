#pragma once
class SpotifyProvider
{
public:
	void Login();
	void ProcessEvents();
	SpotifyProvider();
	~SpotifyProvider();
private:
	int m_next_timeout = 0;
	static sp_session* m_session;
};

