#pragma once
#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <winsock.h>

class Client {
	WSAData m_WSAData;
	SOCKET m_client_socket;
	sockaddr_in m_addr_client;
	char recvbuffer[512] = "";
	char m_LogBuffer[256];
public:
	int m_iResult;
	std::ofstream file;
	Client();
	~Client();
	bool ClientStart();
	void Connect(int port,const char* ipAddress = "localhost");
	void RecieveFile();
	void Disconnect();
};