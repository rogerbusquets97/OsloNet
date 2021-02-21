#include "MyServer.h"

int main()
{
	if (!Oslo::net::utils::sock_init())
	{
		printf("Failed to initialize winsock!");
		return 1;
	}

	Server* server = new Server(9999);

	server->Start();

	server->Run();

	delete server;
	getchar();

	return 0;
}