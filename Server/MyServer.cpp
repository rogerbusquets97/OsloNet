#include <stdio.h>
#include "MyServer.h"
#include "../MessageTypes.h"

Server::Server(uint32 i_port) : server_interface(i_port), m_clients()
{
}

void Server::OnMessage(IP_Endpoint endpoint, Oslo::net::Message& msg)
{
	MessageType type;
	msg.Read<MessageType>(type);

	if (type == MessageType::LoginRequest)
	{
		printf("Server OnMessage Login Request!\n");
		m_clients.push_back(endpoint);

		// We can get endpoint's address and port to store them somehow
		// We probably should return to the client his unique id
		Oslo::net::Message response_msg;
		response_msg.Write<MessageType>(MessageType::LoginResponse);

		// Send response to cliet
		if (!SendToClient(endpoint, response_msg))
		{
			printf("Failed to deliver message to client!");
		}
	}
}

void Server::UpdateClients()
{
	for (auto& client : m_clients)
	{
		// Update clients!
	}
}

