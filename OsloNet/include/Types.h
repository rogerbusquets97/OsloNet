#pragma once

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned long long uint64;
typedef unsigned char uint8;
typedef float float32;

struct IP_Endpoint
{
	uint32 address;
	uint16 port;

	bool operator==(const IP_Endpoint& other)
	{
		return address == other.address && port == other.port;
	}
};