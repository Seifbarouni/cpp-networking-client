#include <iostream>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main()
{
	string ipAddr = "127.0.0.1";
	int port = 54000;

	// Initialize winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start winsock ,Err # " << wsResult;
		return 1;
	}

	// Create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket. Err #" << WSAGetLastError();
		return 1;
	}


	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddr.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server. Err #" << WSAGetLastError();
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	
	char buf[4096];
	string userInput;
	do
	{
		// User input
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0)		
		{
			// Send data
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
			}
		}
		
	} while (userInput.size()>0);

	// closing the connexion
	closesocket(sock);
	WSACleanup();
}
