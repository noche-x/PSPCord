#include <iostream>
#include <winsock2.h>

using namespace std;

extern "C" {
	char test_message[1024] = "hello from the server :)";
}

int main()
{
	// declerations
	WSADATA WSAData;
	SOCKET server, client;
	SOCKADDR_IN serverAddr, clientAddr;

	// start the winsock
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	// create a socket to connect to the client
	server = socket(AF_INET, SOCK_STREAM, 0);

	// setting up the port
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5555);

	// make the client connect to the server
	bind(server, (SOCKADDR*)& serverAddr, sizeof(serverAddr));

	// listen through the socket
	listen(server, 0);

	cout << "Listening for incoming connections..." << endl;

	char buffer[1024]; 

	int clientAddrSize = sizeof(clientAddr);
	if ((client = accept(server, (SOCKADDR*)& clientAddr, &clientAddrSize)) != INVALID_SOCKET)
	{
		cout << "Client connected!" << endl;

		// recieve the clients message
		recv(client, buffer, sizeof(buffer), 0);

		// lets print the clients message
		cout << "Client says: " << buffer << endl;
		
		std::cin >> test_message;

		// send a hello message to the client
		send(client, test_message, sizeof(test_message), 0);

		// clear the *buffer
		memset(buffer, 0, sizeof(buffer));

		// close the socket since we dont use it
		closesocket(client);

		cout << "Client disconnected." << endl;
	}
}