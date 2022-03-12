#pragma once
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include "ExtPort.h"
#define MAX_EXT_PORT 128
#define PORT_NOT_EXIST -3;
#define PORT_EXIST -2
#define BAD_PORT -1
#define SUCCESS 0

class configuration
{
public:
	int Ports[MAX_EXT_PORT];
	int PortCount;
	bool IsFileOK;
	bool IsReceiverOK;

	configuration();
	int AddPort(int new_port);
	int RemovePort(int old_port);
	void SyncPorts(std::vector<ExtPort*>* _myPorts); //Config -> Ports
};