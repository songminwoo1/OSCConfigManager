#pragma comment(lib, "ws2_32")
#pragma comment( lib, "advapi32.lib" )
#include <fstream>
#include <string>
#include "configuration.h"
#include "MallangHTML.h"
#include <winbase.h>
#include <Tlhelp32.h>
#define BUFSIZE 2048
#define OSC_BUFFER_SIZE 1024


CRITICAL_SECTION ConfigWrite;
CRITICAL_SECTION UserOSCSend;
configuration* OSC_Config;
//portforwarder
std::vector<ExtPort*> myPorts;
ExtPort* VRChatPort;
SOCKET sender_toVRC;

DWORD WINAPI ProcessClient(LPVOID _client_sock) {
	SOCKET client_sock = (SOCKET)_client_sock;
	DWORD timeout = 1000;
	setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	int recvLength = 0;
	char buf[BUFSIZE];
	while (true) {
		//std::cout << "wait for recvn\n";
		if ((recvLength = recv(client_sock, buf, BUFSIZE, 0)) == SOCKET_ERROR) {
			//std::cout << "recv error\n"; //mostly, connection closed.
			closesocket(client_sock);
			return 0;
		}
		//std::cout << "received data\n";
		int cout = 0;
		for (int i = 0; i < BUFSIZE - 1; i++) {
			if (buf[i] == '\n') {
				buf[i] = '\0';
				break;
			}
		}

		std::string HttpReq = std::string(buf);
		std::string Method;
		std::string RequestPath;

		size_t breakA = HttpReq.find(' ');
		if (breakA == std::string::npos || breakA < 3) {
			continue;
		}
		else {
			Method = HttpReq.substr(0, breakA);
			//std::cout << "method: " << Method.c_str() << std::endl;

			size_t breakB = HttpReq.find(' ', breakA + 1);
			if (breakB == std::string::npos || breakB - breakA < 2) {
				continue;
			}
			else {
				RequestPath = HttpReq.substr(breakA + 1, breakB - breakA - 1);
			}
		}

		size_t ParmPos = RequestPath.find("?");
		std::string subPath;
		std::string Parameter_name;
		std::string Parameter_value;

		if (RequestPath == "/quit")
			exit(-1);

		subPath = RequestPath.substr(0, ParmPos);

		//std::cout << "sub path: " << subPath.c_str() << std::endl;

		if (ParmPos != std::string::npos) {
			size_t ValuePos = RequestPath.find("=");
			Parameter_name = RequestPath.substr(ParmPos + 1, ValuePos - ParmPos - 1);
			Parameter_value = RequestPath.substr(ValuePos + 1, std::string::npos);
		}

		//std::cout << "parameter: " << Parameter_name << ", " << Parameter_value << std::endl;

		if (Parameter_name == "NewPortnum") {
			int newP = 0;
			try {
				newP = std::stoi(Parameter_value);
			}
			catch (...) {
			}

			int returnMsg = OSC_Config->AddPort(newP);
			if (returnMsg != SUCCESS) {
				//std::cout << "port addition failed\n";
			}
			else {
				OSC_Config->SyncPorts(&myPorts);
				//std::cout << "port added\n";
			}
		}
		else if (Parameter_name == "OldPortnum") {
			int oldP = 0;
			try {
				oldP = std::stoi(Parameter_value);
			}
			catch (...) {
			}

			int returnMsg = OSC_Config->RemovePort(oldP);
			if (returnMsg != SUCCESS) {
				//std::cout << "port removal failed: " << returnMsg << std::endl;
			}
			else {
				OSC_Config->SyncPorts(&myPorts);
				//std::cout << "port removed\n";
			}
		}
		else if (Parameter_name == "OSCCommand") {
			size_t UserInputAddrPos = Parameter_value.find("+");
			if (UserInputAddrPos < 1) {
				goto BROWSER_RETURN;
			}

			std::string _OSCAddr = "/avatar/parameters/" + Parameter_value.substr(0, UserInputAddrPos);
			std::string _OSCData = Parameter_value.substr(UserInputAddrPos + 1, Parameter_value.find("+", UserInputAddrPos + 1));

			//std::cout << "_OSCData: " << _OSCData.c_str() << std::endl;

			int ValidOSCData = 0;
			try {
				ValidOSCData = std::stoi(_OSCData);
			}
			catch (...) {
				goto BROWSER_RETURN;
			}
			int OSCData = htonl(ValidOSCData);
			//std::cout << "OSCData: " << OSCData << std::endl;

			const char* OSCAddr = _OSCAddr.c_str();

			int message_Length = ((int)strlen(OSCAddr) / 4) * 4 + 12;

			char* message = (char*)malloc(message_Length);
			if (message == nullptr) {
				exit(0);
			}
			if (message_Length < 12) {
				free(message);
				exit(0);
			}
			for (int i = 0; i < strlen(OSCAddr); i++) {
				message[i] = OSCAddr[i];
			}
			for (int i = (int)strlen(OSCAddr); i < message_Length - 8; i++) {
				message[i] = 0;
			}
			message[message_Length - 8] = ',';
			message[message_Length - 7] = 'i';
			message[message_Length - 6] = 0;
			message[message_Length - 5] = 0;
			*(int*)(message + message_Length - 4) = OSCData;

			EnterCriticalSection(&UserOSCSend);
			VRChatPort->Send(sender_toVRC, (char*)message, message_Length);
			LeaveCriticalSection(&UserOSCSend);
			free(message);
		}


	BROWSER_RETURN: // be careful!
		if (Method == "GET") {
			//url compare.
			if (subPath == "/") {
				//do whatever here.
				send(client_sock, MallangBanner, sizeof(MallangBanner), 0);
				send(client_sock, MallangVideo, sizeof(MallangVideo), 0);
				send(client_sock, MallangEnder, sizeof(MallangEnder), 0);
			}
			else if (subPath == "/portforwarding") {
				std::string html1 = std::string(HTML1);
				std::string html2 = std::string(HTML2);
				//std::string html_mallang = std::string(HTML_Mallang);
				//send back data
				std::string OpenPort;
				EnterCriticalSection(&ConfigWrite);
				for (int i = 0; i < OSC_Config->PortCount; i++) {
					OpenPort += std::to_string(OSC_Config->Ports[i]);
					OpenPort.push_back(' ');
				}
				if (OSC_Config->PortCount == 0) {
					OpenPort = "no open ports";
				}
				LeaveCriticalSection(&ConfigWrite);
				//std::cout << "sending data\n";
				send(client_sock, MallangBanner, sizeof(MallangBanner), 0);
				send(client_sock, Portforwarding_htmlA, sizeof(Portforwarding_htmlA), 0);
				send(client_sock, OpenPort.c_str(), OpenPort.size(), 0);
				send(client_sock, Portforwarding_htmlB, sizeof(Portforwarding_htmlB), 0);
				send(client_sock, MallangEnder, sizeof(MallangEnder), 0);
			} //for general main path request
			else if (subPath == "/parmList") {
				send(client_sock, MallangBanner, sizeof(MallangBanner), 0);
				send(client_sock, MallangOSC_commandList, sizeof(MallangOSC_commandList), 0);
				send(client_sock, MallangEnder, sizeof(MallangEnder), 0);
			}
			else if (subPath == "/SendOSC") {
				send(client_sock, MallangBanner, sizeof(MallangBanner), 0);
				send(client_sock, UserOSCSend_html, sizeof(UserOSCSend_html), 0);
				send(client_sock, MallangEnder, sizeof(MallangEnder), 0);
			}
			else {
				//std::cout << "undefined request\n";
				closesocket(client_sock);
				return 0;
			}
		}
	}
}

DWORD WINAPI TCPListener(LPVOID tempInput)
{
	//std::cout << "server started!\n";
	SOCKET tcplistener = socket(AF_INET, SOCK_STREAM, 0);
	if (tcplistener == INVALID_SOCKET) {
		return -1;
	}

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serverAddr.sin_port = htons(80);

	if (bind(tcplistener, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		return -1;
	}
	//std::cout << "serverAddr binded\n";

	if (listen(tcplistener, SOMAXCONN) == SOCKET_ERROR) {
		return -2;
	}
	//std::cout << "server listening\n";
	

	SOCKET client_sock;
	SOCKADDR_IN clientAddr;
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	int addrlen = sizeof(clientAddr);
	HANDLE ClientProcessor;

	while (true) {
		client_sock = accept(tcplistener, (SOCKADDR*)&clientAddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			closesocket(client_sock);
			continue;
		}

		//std::cout << "client connected\n";

		ClientProcessor = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (ClientProcessor == NULL) {
			closesocket(client_sock);
			continue;
		}
		CloseHandle(ClientProcessor);
	}

	closesocket(tcplistener);

	WSACleanup();
	return 0;
}

bool isRunning(LPCWSTR pName)
{
	HWND hwnd;
	hwnd = FindWindow(NULL, pName);
	if (hwnd != 0) {
		return true;
	}
	else {
		return false;
	}
}

BOOL MySystemShutdown(int t)
{
	HANDLE hToken;              // handle to process token 
	TOKEN_PRIVILEGES tkp;       // pointer to token structure 

	// Get the current process token handle so we can get shutdown 
	// privilege. 

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return FALSE;

	// Get the LUID for shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	// Cannot test the return value of AdjustTokenPrivileges. 

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	// Display the shutdown dialog box and start the countdown. 

	char tempMSG[] = "mallangOSC_Shutdown";
	LPSTR mallangMSG = tempMSG;

	std::cout << "shutdown error message: " << InitiateSystemShutdownA(NULL, mallangMSG, t, FALSE, FALSE);

	// Disable shutdown privilege. 

	tkp.Privileges[0].Attributes = 0;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);
	return TRUE;
}

BOOL MyCancelShutdown()
{
	HANDLE hToken;              // handle to process token 
	TOKEN_PRIVILEGES tkp;       // pointer to token structure 

	// Get the current process token handle so we can get shutdown 
	// privilege. 

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return FALSE;

	// Get the LUID for shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	// Cannot test the return value of AdjustTokenPrivileges. 

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	// Display the shutdown dialog box and start the countdown. 

	char tempMSG[] = "mallangOSC_Shutdown";
	LPSTR mallangMSG = tempMSG;

	std::cout << "shutdown error message: " << AbortSystemShutdownA(NULL);

	// Disable shutdown privilege. 

	tkp.Privileges[0].Attributes = 0;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);
	return TRUE;
}

bool IsMallangOSC(char buf[OSC_BUFFER_SIZE + 1], int receivedSize) {
	static int LastCommand = 0;
	buf[receivedSize] = '\0';
	if (strcmp(buf, "/avatar/parameters/mallangOSC") == 0) {

		int dataPos = 0;
		char value[4];
		ZeroMemory(value, sizeof(value));

		for (int i = 0; i < 4; i++) {
			value[i] = buf[receivedSize - 1 - i];
		}

		switch (LastCommand)
		{
		case 101:
			switch (*(int*)value)
			{
			case 1:
				MySystemShutdown(60); //1min
				return true;

			case 2:
				MySystemShutdown(300); //5min
				return true;

			case 3:
				MySystemShutdown(900); //15min
				return true;

			case 4:
				MySystemShutdown(1800); //30min
				return true;
			}
		default:
			break;
		}

		switch (*(int*)value) {
		case 0:
			LastCommand = 0;
			return true;
		case 100:
			MySystemShutdown(5);
			LastCommand = 100;
			return true;

		case 101:
			LastCommand = 101;
			return true;

		case 102:
			MyCancelShutdown();
			LastCommand = 102;
			return true;
		}
	}
	return false;
}

int main() {
	const char* szUniqueNamedMutex = "mallangOSC_Controller";
	HANDLE mallangHandle = CreateMutex(NULL, TRUE, (LPCWSTR)szUniqueNamedMutex);
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		// Program already running somewhere
		return -1; // Exit program
	}
	if (mallangHandle == NULL) {
		return -2;
	}


	//now, main program starts.
	InitializeCriticalSection(&ConfigWrite);
	InitializeCriticalSection(&UserOSCSend);

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		DeleteCriticalSection(&ConfigWrite);
		DeleteCriticalSection(&UserOSCSend);
		closesocket(sender_toVRC);
		ReleaseMutex(mallangHandle);
		CloseHandle(mallangHandle);
		return 1;
	}

	VRChatPort = new ExtPort(9000);

	HANDLE TcpListener = CreateThread(NULL, 0, TCPListener, 0, 0, NULL);

	// open configuration file
	OSC_Config = new configuration();
	sender_toVRC = socket(AF_INET, SOCK_DGRAM, 0);

	//now, socket io starts.
	SOCKET receiver_fromVRC = socket(AF_INET, SOCK_DGRAM, 0);
	if (receiver_fromVRC == INVALID_SOCKET) {
		//receiver is not okay. this is alreay set in config.
		DeleteCriticalSection(&ConfigWrite);
		DeleteCriticalSection(&UserOSCSend);
		closesocket(sender_toVRC);
		ReleaseMutex(mallangHandle);
		CloseHandle(mallangHandle);
		return 1;
	}

	SOCKADDR_IN localAddr;
	ZeroMemory(&localAddr, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	localAddr.sin_port = htons(9001); //where we receive data from.

	if (bind(receiver_fromVRC, (SOCKADDR*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
		//receiver is not okay. this is alreay set in config.
		DeleteCriticalSection(&ConfigWrite);
		DeleteCriticalSection(&UserOSCSend);
		closesocket(sender_toVRC);
		ReleaseMutex(mallangHandle);
		CloseHandle(mallangHandle);
		return -1;
	}
	OSC_Config->IsReceiverOK = true; //now, receiver is fully fuctional.
	int receivedSize = 0;
	char buf[OSC_BUFFER_SIZE + 1];

	//just a dump place for receve function.
	SOCKADDR_IN peerAdddr;
	ZeroMemory(&peerAdddr, sizeof(peerAdddr));
	int peerAddrLen = sizeof(peerAdddr);

	//first, sync port from config.
	OSC_Config->SyncPorts(&myPorts);
	while (true) {
		receivedSize = recvfrom(receiver_fromVRC, buf, OSC_BUFFER_SIZE, 0, (SOCKADDR*)&peerAdddr, &peerAddrLen);
		//std::cout << "VRChat Data receive\n";

		if (IsMallangOSC(buf, receivedSize) == false) {
			EnterCriticalSection(&ConfigWrite);
			for (ExtPort* myPort : myPorts) {
				//std::cout << "forwarded to: " << myPort->port << std::endl;
				myPort->Send(receiver_fromVRC, buf, receivedSize);
			}
			LeaveCriticalSection(&ConfigWrite);
		}
	}

	DeleteCriticalSection(&ConfigWrite);
	DeleteCriticalSection(&UserOSCSend);
	closesocket(sender_toVRC);
	ReleaseMutex(mallangHandle);
	CloseHandle(mallangHandle);
	return 0;
}