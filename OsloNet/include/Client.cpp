#include <WinSockUtils.h>
#include <Message.h>
#include "Client.h"

namespace Oslo
{
	namespace net
	{
		bool client_interface::Start()
		{
			m_socket = utils::create_udp_socket();

			return m_socket != INVALID_SOCKET;
		}

		bool client_interface::IsReady()
		{
			return m_socket != INVALID_SOCKET;
		}

		bool client_interface::SendToServer(Message& msg)
		{
			SOCKADDR_IN server_adress;
			server_adress.sin_family = AF_INET;
			server_adress.sin_port = htons(m_serverPort);
			server_adress.sin_addr.S_un.S_addr = inet_addr(m_serverAddress);

			return sendto(m_socket, msg.body.GetBufferPtr(), msg.body.GetSize(), 0, (SOCKADDR*)& server_adress, sizeof(server_adress)) != SOCKET_ERROR;
		}

		void client_interface::PeekMessages()
		{
			IP_Endpoint from;
			uint32 bytes_received;

			while(true)
			{
				int flags = 0;
				SOCKADDR_IN from;
				int from_size = sizeof(from);
				char buffer[1024];
				int bytes_received = recvfrom(m_socket, buffer, 1024, flags, (SOCKADDR*)&from, &from_size);

				if (bytes_received == SOCKET_ERROR)
				{
					int error = WSAGetLastError();
					printf("recvfrom returned SOCKET_ERROR, Last Error: %d\n", error);
					break;
				}

				if (bytes_received > 0)
				{
					Message msg(buffer, bytes_received);
					OnMessage(msg);
				}
			}
		}
	}
}
