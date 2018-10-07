#include "Client.h"
#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")
#include <locale>
#define UNICODE
Client::Client() {
	std::locale::global(std::locale(""));
}
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
	while (RecieveFile());
}

bool Client::RecieveFile() {
	static int sum = 0;
	static std::streampos position = 0;
	file.seekp(position);
	long filesize = 706;
	position = filesize;
	m_iResult = recv(m_client_socket, recvbuffer, filesize, 0);

	if (m_iResult > 0) {
		file.open("picture1.png", std::ios::binary | std::ios::ate | std::ios::out);
		if (file.is_open()) {
			file.write(recvbuffer, filesize);
			file.flush();
			std::cout << "Recieve bytes : " << m_iResult << std::endl;
			sum += m_iResult;
			file.close();
		}
		else {
			std::cout << "File didn't open. Error :" << GetLastError() << std::endl;
			return 0;
		}
	}
	else if (m_iResult == 0) {
		std::cout << "Connection is closing." << std::endl;
		std::cout << "Total bytes : " << sum << std::endl;
		return 0;
		}
	else {
		std::cout << "Recieve is failed. Error :" << WSAGetLastError() << std::endl;
		return 0;
		}
	return 1;
}

void Client::Disconnect() {
	
	closesocket(m_client_socket);
	std::cout << "Disconnected.\n";
	WSACleanup();
	std::cin.get();
}