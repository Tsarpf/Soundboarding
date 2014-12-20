#include "Server.h"
int main()
{
	const char *username = NULL;
	const char *password = NULL;
	int opt;

	while ((opt = getopt(argc, argv, "u:p:l:d")) != EOF) {
		switch (opt) {
		case 'u':
			username = optarg;
			break;

		case 'p':
			password = optarg;
			break;

		case 'l':
			g_listname = optarg;
			break;

		case 'd':
			g_remove_tracks = 1;
			break;

		default:
			exit(1);
		}
	}

	if (!username || !password || !g_listname) {
		usage(argv[0]);
		std::cout << "username passsowrd list name missing or something";
		exit(1);
	}
	k
	Server server;

	server.Run();

	return 0;
}