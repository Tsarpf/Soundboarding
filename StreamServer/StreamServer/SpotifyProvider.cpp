#include "SpotifyProvider.h"
#include <stdint.h>
#include <libspotify/api.h>
#include <iostream>

SpotifyProvider::SpotifyProvider()
{
	extern const uint8_t g_appkey[];
	extern const size_t g_appkey_size;
	sp_session_config spconfig;

	memset(&spconfig, 0, sizeof(spconfig));
	spconfig.api_version = SPOTIFY_API_VERSION;

	spconfig.cache_location = "tmp";
	spconfig.settings_location = "tmp";

	spconfig.application_key = g_appkey;
	spconfig.application_key_size = g_appkey_size;

	spconfig.user_agent = "jukebewkx";

	//spconfig.callbacks = 0;
	spconfig.callbacks = &session_callbacks;

	spconfig.compress_playlists = 0;

	sp_session *sp;
	sp_error err;
	int next_timeout = 0;

	//audio_init(&g_audiofifo);

	/* Create session */
	spconfig.application_key_size = g_appkey_size;

	pthread_mutex_init(&g_notify_mutex, NULL);
	pthread_cond_init(&g_notify_cond, NULL);

	for (int i = 0; i < 322; i++)
	{
		char penis = g_appkey[i];
	}

	err = sp_session_create(&spconfig, &sp);

	if (SP_ERROR_OK != err) {
	fprintf(stderr, "Unable to create session: %s\n",
		sp_error_message(err));
	exit(1);
	}
}


SpotifyProvider::~SpotifyProvider()
{
}


void SpotifyProvider::Login()
{
	sp_session_login(sp, username, password, 0, NULL);

}

void SpotifyProvider::ProcessEvents()
{
	if (m_next_timeout == 0)
	{
		sp_session_process_events(m_session, &m_next_timeout);
	}
}








