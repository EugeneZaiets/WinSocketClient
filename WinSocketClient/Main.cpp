#include "Client.h"

int main() {
	Client client;
	client.ClientStart();
	client.Connect(8888, "localhost");
	system("pause");
	client.Disconnect();
	return 0;
}