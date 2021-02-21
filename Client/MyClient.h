#pragma once
#include <OsloNet.h>

class Client : public Oslo::net::client_interface
{
public:

	Client(const char* i_serverAddress, uint32 i_serverPort);

	~Client() = default;

	void Run();
	
protected:
	void OnMessage(Oslo::net::Message& msg) override;
};