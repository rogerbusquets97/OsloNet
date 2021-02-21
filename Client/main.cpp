#include "MyClient.h"

int main()
{
	Oslo::net::utils::sock_init();

	// Point to local IP address as Server
	// This may change to point to the server address
	Client* client = new Client("127.0.0.1", 9999);
	client->Run();
	delete client;
	return 0;
}