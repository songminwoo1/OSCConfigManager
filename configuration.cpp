#include "configuration.h"
#define mallangPath "C:/mallangConfig/ports.txt"

extern CRITICAL_SECTION ConfigWrite;

configuration::configuration() {
	ZeroMemory(&Ports, sizeof(Ports));
	PortCount = 0;

	std::fstream configFile;
	//first, need to check file existance!! - may assume we have one;
	FILE_OPEN:
	configFile.open(mallangPath);
	if (configFile.is_open()) {
		IsFileOK = true;
		std::string line;
		while (getline(configFile, line)) {
			int PortNum = std::stoi(line);
			if (PortNum == 0 || PortNum == 9001 || PortNum == 9000) {
				continue;
			}
			else {
				Ports[PortCount] = PortNum;
				PortCount++; //total open external port count.
			}
		}
		configFile.close();
	}
	else {
		//std::cout << "no file found\n";
		IsFileOK = false;
		std::ofstream configFile(mallangPath);
		goto FILE_OPEN;
	}
	IsReceiverOK = false;
}

int configuration::AddPort(int new_port) {
	EnterCriticalSection(&ConfigWrite);
	for (int i = 0; i < PortCount; i++) {
		if (Ports[i] == new_port) {
			LeaveCriticalSection(&ConfigWrite);
			return PORT_EXIST;
		}
	}
	if (new_port == 0 || new_port == 9001 || new_port == 9000) {
		LeaveCriticalSection(&ConfigWrite);
		return BAD_PORT;
	}
	Ports[PortCount] = new_port;
	PortCount++;

	//also, write to the port file.
	std::fstream configFile;
	configFile.open(mallangPath, std::fstream::out);
	if (configFile.is_open()) {
		for (int i = 0; i < PortCount; i++) {
			configFile << std::to_string(Ports[i]) << std::endl;
		}
	}
	configFile.close();
	LeaveCriticalSection(&ConfigWrite);
	return SUCCESS;
}

int configuration::RemovePort(int old_port) {
	EnterCriticalSection(&ConfigWrite);
	int tmp = PORT_NOT_EXIST;
	if (PortCount == 0) {
		LeaveCriticalSection(&ConfigWrite);
		return PORT_NOT_EXIST;
	}
	for (int i = 0; i < PortCount-1; i++) {
		if (Ports[i] == old_port) {
			tmp = SUCCESS;
		}
		if (tmp == SUCCESS) {
			Ports[i] = Ports[i + 1];
		}
		if (i == PortCount - 2 && tmp == SUCCESS) {
			PortCount--;
			Ports[PortCount] = 0;
			goto AFTER_SUCCESS;
		}
	}
	if (Ports[PortCount - 1] == old_port) {
		Ports[PortCount - 1] = 0;
		PortCount--;
		goto AFTER_SUCCESS;
	}
	LeaveCriticalSection(&ConfigWrite);
	return PORT_NOT_EXIST;

	AFTER_SUCCESS:
	//write to the port file.
	std::fstream configFile;
	configFile.open(mallangPath, std::fstream::out);
	if (configFile.is_open()) {
		for (int i = 0; i < PortCount; i++) {
			configFile << std::to_string(Ports[i]) << std::endl;
		}
	}
	configFile.close();
	LeaveCriticalSection(&ConfigWrite);
	return SUCCESS;
}

void configuration::SyncPorts(std::vector<ExtPort*>* _myPorts) {
	EnterCriticalSection(&ConfigWrite);
	_myPorts->~vector();

	std::vector<ExtPort*>* newPorts = new std::vector<ExtPort*>;
	for (int i = 0; i < PortCount; i++) {
		ExtPort* newExt = new ExtPort(Ports[i]);
		_myPorts->push_back(newExt);
	}
	_myPorts = newPorts;
	LeaveCriticalSection(&ConfigWrite);
}