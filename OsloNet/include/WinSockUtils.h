#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <Types.h>
#include <stdio.h>

//It will only work for windows
#ifndef WIN32
#error "Oslo Net only supports windows for now."
#endif

#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)

namespace Oslo
{
	namespace net
	{
		namespace utils
		{
			LONGLONG time_since(LARGE_INTEGER t, LARGE_INTEGER frequency);

			// Initialize Winsock2
			// Return true if the initialization succeeded
			bool sock_init();

			//Shuts down Winsock2
			//Return true if cleanup succeeded
			bool sock_cleanup();

			//Creates a socket that follows the udp protocol
			//May be invalid so must be checked with INVALID_SOCKET
			SOCKET create_udp_socket();

			bool socket_send(SOCKET sock, char* message, uint32 message_size, IP_Endpoint* endpoint);

			bool socket_receive(SOCKET sock, char* buffer, uint32 buffer_size, IP_Endpoint* out_from, uint32* out_bytes_received);

			void set_non_blocking_socket(SOCKET sock, u_long enabled);

			//Binds a socket to the specified port
			//Returns true if the binding succeeded.
			bool bind_socket(SOCKET i_socket, unsigned short i_port);
		}
	}
}