#pragma once
#include <Windows.h>
#include <iostream>
class ExtPort
{
public:
	int port;
	SOCKADDR_IN ExtAddr;

	ExtPort(int Pn);
	void Send(SOCKET _sender, char* data, size_t recvSize);
};

