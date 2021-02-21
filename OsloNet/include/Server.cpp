#include "Server.h"

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

			UINT sleep_granularity_ms = 1;
			bool sleep_granularity_was_set = timeBeginPeriod(sleep_granularity_ms) == TIMERR_NOERROR;
			LARGE_INTEGER clock_frequency;
			QueryPerformanceCounter(&clock_frequency);

			while (m_running)
			{
				LARGE_INTEGER tick_start_time;
				QueryPerformanceCounter(&tick_start_time);

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

				/*LONGLONG time_taken_s = utils::time_since(tick_start_time, clock_frequency);
				while (time_taken_s < SECONDS_PER_TICK)
				{
					if (sleep_granularity_was_set)
					{
						DWORD time_to_wait_ms = DWORD((SECONDS_PER_TICK - time_taken_s) * 1000);
						if (time_to_wait_ms > 0)
						{
							Sleep(time_to_wait_ms);
						}
					}

					time_taken_s = utils::time_since(tick_start_time, clock_frequency);
				}*/
			}
		}
	}
}