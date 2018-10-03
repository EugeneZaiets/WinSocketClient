#include "Client.h"
#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")

Client::Client() {}
Client::~Client() {}

bool Client::ClientStart() {
	if (WSAStartup(MAKEWORD(2, 2), &m_WSAData) == NULL) {
		std::cout << "Client WSAStartup is successful\n";
		wsprintf(m_LogBuffer, "Client %s is %s", m_WSAData.szDescription, m_WSAData.szSystemStatus);
		m_WSAData.iMaxUdpDg = 512;
		
		std::cout << "iMaxUdpDg size : " << m_WSAData.iMaxUdpDg << " bytes." << std::endl;
		std::cout << m_LogBuffer << std::endl;
	}
	else {
		std::cout << "WSAStartUp Error : " << WSAGetLastError();
		std::cin.get();
		return false;
	}
	return true;
}

void Client::Connect(int port, const char* ipAddress)
{
	m_client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_client_socket == INVALID_SOCKET) {
		std::cout << "Error : INVALID_SOCKET | Last Error : " << WSAGetLastError() << "\n";
		return;
	}
	m_addr_client.sin_family = AF_INET;
	m_addr_client.sin_port = htons(port);

	PHOSTENT phe;
	phe = gethostbyname(ipAddress);
	if (phe == NULL) {
		std::cout << "GetHostByName Error : " << WSAGetLastError() << std::endl;
		closesocket(m_client_socket);
		return;
	}
	memcpy((char FAR*) &m_addr_client.sin_addr, phe->h_addr_list[0], phe->h_length);
	if (connect(m_client_socket, (sockaddr*)&m_addr_client, sizeof(m_addr_client)) < 0) {
		std::cout << "Connection Error : " << WSAGetLastError() << std::endl;
		return;
	}
	else {
		std::cout << "Connected to Server. IP - " << inet_ntoa(m_addr_client.sin_addr) << "\n";
		
	}
	while (true)
	{
		RecieveFile();
	}
}

void Client::RecieveFile() {
	int sum = 0;
	file->open("C:\\Users\gekaz\source\repos\WinSocketClient\WinSocketClient\picture1.png", std::ios::binary | std::ios::ate);
	if (file->is_open()) {
		long filesize = file->tellp();
		char* buffer = new char[filesize];
		file->seekp(std::ios::beg);
		file->write(buffer, filesize);
		m_iResult = recv(m_client_socket, recvbuffer, sizeof(recvbuffer), 0);
		if (m_iResult > 0) {
			std::cout << "Recieve bytes : " << m_iResult << std::endl;
			sum += m_iResult;
		}
		else if (m_iResult == 0) {
			std::cout << "Connection closed." << std::endl;
			std::cout << "Total bytes : " << sum << std::endl;
			std::cout << "Recieved Buffer: " << recvbuffer << std::endl;
			return;
		}
		else  std::cout << "Recieve is failed. Error :" << WSAGetLastError() << std::endl;
	}
	else std::cout << "File didn't open. Error :" << GetLastError() << std::endl;
}

void Client::Disconnect() {
	
	closesocket(m_client_socket);
	std::cout << "Disconnected.\n";
	WSACleanup();
	std::cin.get();
}