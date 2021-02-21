#pragma once

#include <vector>
#include <OsloNet.h>

class Server : public Oslo::net::server_interface
{
public:

	Server(uint32 i_port);
	~Server() = default;

protected:

	void OnMessage(IP_Endpoint endpoint, Oslo::net::Message& msg) override;
	void UpdateClients() override;

private:

	std::vector<IP_Endpoint> m_clients;
};