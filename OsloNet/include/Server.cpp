#include "Server.h"
#include <chrono>
#include <thread>

namespace Oslo
{
	namespace net
	{
		bool server_interface::SendToClient(IP_Endpoint endpoint, Message& msg)
		{
			int flags = 0;
			SOCKADDR_IN to;
			to.sin_family = AF_INET;
			to.sin_port = endpoint.port;
			to.sin_addr.S_un.S_addr = endpoint.address;
			int to_length = sizeof(to);

			return sendto(m_socket, msg.body.GetBufferPtr(), msg.body.GetSize(), flags, (SOCKADDR*)& to, to_length) != SOCKET_ERROR;
		}

		bool server_interface::Start()
		{
			m_socket = utils::create_udp_socket();

			if (m_socket == INVALID_SOCKET)
				return false;

			if (!utils::bind_socket(m_socket, m_port))
			{
				printf("[Server]: Socket bind failed\n");
				return false;
			}

			utils::set_non_blocking_socket(m_socket, 1);

			m_running = true;
			return true;
		}


		bool server_interface::IsRunning() const
		{
			return m_running;
		}

		void server_interface::Stop()
		{
			bool m_running = false;
		}

		void server_interface::Run()
		{
			if (!m_running)
				return;

			std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();

			while (m_running)
			{
				while (true)
				{
					int flags = 0;
					SOCKADDR_IN from;
					int from_size = sizeof(from);

					char buffer[1024];

					int bytes_recieved = recvfrom(m_socket, buffer, 1024, flags, (SOCKADDR*)& from, &from_size);

					if (bytes_recieved == SOCKET_ERROR)
					{
						int error = WSAGetLastError();
						if (error != WSAEWOULDBLOCK)
						{
							printf("recvfrom returned SOCKET_ERROR, Last Error: %d\n", error);
						}

						break;
					}

					Message msg(buffer, bytes_recieved);

					IP_Endpoint from_endpoint;
					from_endpoint.address = from.sin_addr.S_un.S_addr;
					from_endpoint.port = from.sin_port;

					OnMessage(from_endpoint, msg);
				}

				UpdateClients();

				t+= std::chrono::milliseconds(33);
				std::this_thread::sleep_until(t);
			}
		}
	}
}