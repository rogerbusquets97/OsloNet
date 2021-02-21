#pragma once
#include <WinSockUtils.h>
#include <Message.h>

namespace Oslo
{
	namespace net
	{
		const uint32 TICKS_PER_SECOND = 60;
		const float32 SECONDS_PER_TICK = 1.0f;
		const uint16 MAX_CONNECTIONS = 32;

		// Simple server interface that handles socket creation and message receive/send
		class server_interface
		{
		public:

			server_interface(uint16 i_port) : m_port(i_port), m_running(false), m_socket(INVALID_SOCKET)
			{
			}

			virtual ~server_interface()
			{
				Stop();
			}

		protected:

			// OnMessage callback to be overriden by user. Sends the message and the sender.
			// It is up to the user to decide what to do with that message
			virtual void OnMessage(IP_Endpoint endpoint, Message& msg)
			{
				
			}

			// Called after all messages had been processed.
			// User should update any data that must be sent to the client and send it.
			virtual void UpdateClients()
			{
				
			}

			//Sends a message to the specified end point.
			//Returns true if the sending was succesfull
			bool SendToClient(IP_Endpoint endpoint, Message& msg);

		public:

			bool Start();
			
			void Run();
			
			void Stop();

			bool IsRunning() const;

		private:

			uint16 m_port;
			bool m_running;
			SOCKET m_socket;
		};
	}
}