#include "MyClient.h"
#include "../MessageTypes.h"

Client::Client(const char* i_serverAddress, uint32 i_serverPort) : Oslo::net::client_interface(i_serverAddress, i_serverPort)
{
}

void Client::Run()
{
	//Initialize everything to talk to server
	if (!Start())
	{
		printf("Failed to start client!\n");
		return;
	}

	if (!IsReady())
		return;

	printf("Write login request\n");
	Oslo::net::Message msg;
	msg.Write<MessageType>(MessageType::LoginRequest);

	printf("Send request to server\n");
	if (!SendToServer(msg))
	{
		printf("Failed to send message to server!\n");
		return;
	}

	while (true)
	{
		PeekMessages();
	}
}

void Client::OnMessage(Oslo::net::Message& msg)
{
	MessageType type;
	msg.Read<MessageType>(type);
	if (type == MessageType::LoginResponse)
	{
		printf("Server responded to login request!");
	}
}
