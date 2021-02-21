#pragma once
#include <Types.h>

namespace Oslo
{
	namespace net
	{
		struct Message;

		class client_interface
		{
		public:

			client_interface(const char* i_serverAddress, uint32 i_serverPort) : 
				m_serverAddress(i_serverAddress), 
				m_serverPort(i_serverPort), 
				m_socket(~0)
			{
			}

			virtual ~client_interface()
			{
			}

		protected:

			// Creates the socket
			// Returns true if it was created succesfully
			bool Start();

			//Returns if the client has a valid socket, hence it can start talking to the server
			bool IsReady();

			//Send a message to the server specified on construction
			bool SendToServer(Message& msg);

			virtual void OnMessage(Message& msg)
			{
			}

			// Must be called in the main loop to listen for messages
			// If any message is received the OnMessage method will be triggered.
			void PeekMessages();

		private:

			const char* m_serverAddress;
			uint32 m_serverPort;
			SOCKET m_socket;
		};
	}
}