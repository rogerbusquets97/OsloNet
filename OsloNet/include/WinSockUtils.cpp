#include "WinSockUtils.h"

namespace Oslo
{
	namespace net
	{
		namespace utils
		{
			LONGLONG time_since(LARGE_INTEGER t, LARGE_INTEGER frequency)
			{
				LARGE_INTEGER now;
				QueryPerformanceCounter(&now);

				return (now.QuadPart - t.QuadPart) / frequency.QuadPart;
			}

			bool sock_init()
			{
				WORD winsock_version = 0x202;
				WSADATA winsock_data;

				if (WSAStartup(winsock_version, &winsock_data))
				{
					return false;
				}

				return true;
			}

			bool sock_cleanup()
			{
				return !WSACleanup();
			}

			SOCKET create_udp_socket()
			{
				int address_family = AF_INET;
				int type = SOCK_DGRAM;
				int protocol = IPPROTO_UDP;

				SOCKET sock = socket(address_family, type, protocol);

				BOOL bNewBehavior = FALSE;
				DWORD dwBytesReturned = 0;
				WSAIoctl(sock, SIO_UDP_CONNRESET, &bNewBehavior, sizeof bNewBehavior, NULL, 0, &dwBytesReturned, NULL, NULL);

				return sock;
			}

			bool socket_send(SOCKET sock, char* message, uint32 message_size, IP_Endpoint* endpoint)
			{
				SOCKADDR_IN server_address;
				server_address.sin_family = AF_INET;
				server_address.sin_addr.S_un.S_addr = htonl(endpoint->address);
				server_address.sin_port = htons(endpoint->port);
				int server_address_size = sizeof(server_address);

				if (sendto(sock, (const char*)message, message_size, 0, (SOCKADDR*)& server_address, server_address_size) == SOCKET_ERROR)
				{
					printf("Sendto failed: %d\n", WSAGetLastError());
					return false;
				}

				return true;
			}

			bool socket_receive(SOCKET sock, char* buffer, uint32 buffer_size, IP_Endpoint* out_from, uint32* out_bytes_received)
			{
				int flags = 0;
				SOCKADDR_IN from;
				int from_size = sizeof(from);
				int bytes_received = recvfrom(sock, buffer, buffer_size, flags, (SOCKADDR*)& from, &from_size);

				if (bytes_received == SOCKET_ERROR)
				{
					int error = WSAGetLastError();
					if (error != WSAEWOULDBLOCK)
					{
						printf("recvfrom returned SOCKET_ERROR, Last Error: %d\n", error);
					}

					return false;
				}

				*out_from = {};
				out_from->address = ntohl(from.sin_addr.S_un.S_addr);
				out_from->port = ntohs(from.sin_port);

				*out_bytes_received = bytes_received;

				return true;
			}

			void set_non_blocking_socket(SOCKET sock, u_long enabled)
			{
				ioctlsocket(sock, FIONBIO, &enabled);
			}

			//Binds a socket to the specified port
			//Returns true if the binding succeeded.
			bool bind_socket(SOCKET i_socket, unsigned short i_port)
			{
				SOCKADDR_IN local_address;
				local_address.sin_family = AF_INET;
				local_address.sin_port = htons(i_port);
				local_address.sin_addr.s_addr = INADDR_ANY;

				return bind(i_socket, (SOCKADDR*)& local_address, sizeof(local_address)) != SOCKET_ERROR;
			}
		}
	}
}