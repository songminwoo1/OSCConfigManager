#include "ExtPort.h"

ExtPort::ExtPort(int Pn) {
	port = Pn;
	ZeroMemory(&ExtAddr, sizeof(ExtAddr));
	ExtAddr.sin_family = AF_INET;
	ExtAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	ExtAddr.sin_port = htons(port); //where we send data to.
	//std::cout << port << "port set\n";
}

void ExtPort::Send(SOCKET _sender, char* data, size_t recvSize) {
	sendto(_sender, data, recvSize, 0, (SOCKADDR*)&ExtAddr, sizeof(ExtAddr));
}