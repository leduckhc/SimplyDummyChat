#include "pch.h"
//#include "ChatClient.h"
//#include <Windows.h>
//#include <WS2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


bool CppCLRWinformsProjekt::ChatClient::Init(const char* host)
{
	int iserr; // return value, 0 == ok, (>=1) == error
	fout = fopen("output.debug.txt", "w");

	// Initialize Winsock
	WSADATA wsaData;
	WORD ver = MAKEWORD(2, 2);
	iserr = WSAStartup(ver, &wsaData);
	if (iserr != 0)
	{
		fprintf(fout, "WSAStartup failed with error: %d\n", iserr);
		return false;
	}

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	struct addrinfo* result = nullptr;
	iserr = getaddrinfo(host, DEFAULT_PORT, &hints, &result);
	if (iserr != 0) // error
	{
		fprintf(fout, "getaddrinfo failed with error: %d\n", iserr);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (struct addrinfo* ptr = result;
		ptr != nullptr;
		ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);// create socket
		if (ConnectSocket == INVALID_SOCKET) {
			fprintf(fout, "socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}

		// Connect to server.
		iserr = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iserr == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	// Freeup some unneccessary memory
	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		fprintf(fout, "Unable to connect to server!\n");
		WSACleanup();
		return false;
	}

	// https://stackoverflow.com/questions/30395258/setting-timeout-to-recv-function/30402286
	// Set timeouts for receive and sending
	DWORD timeout = 100; // milisecs
	setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	setsockopt(ConnectSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

	recvbuf = new char[buflen];
	is_closed = false;
	return true;
}

bool CppCLRWinformsProjekt::ChatClient::Send(const char* msg)
{
	int bytes = send(ConnectSocket, msg, (int)strlen(msg), 0);
	if (bytes <= 0) // error
	{
		if (WSAGetLastError() != WSAETIMEDOUT)
		{
			fprintf(fout, "send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return false;
		}
	}
	
	fprintf(fout, "Bytes Sent: %d\n", bytes);
	return true;
}

bool CppCLRWinformsProjekt::ChatClient::Receive()
{
	// Receive until the peer closes the connection
	//DWORD timeout = 0.2 * 1000; // nanoseconds
	//setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	ZeroMemory(recvbuf, buflen); // vynulovat recvbuf
	int bytes = recv(ConnectSocket, recvbuf, buflen, 0);
	if (bytes > 0) // ok
	{
		//printf("Bytes received: %d\n", bytes);
		fprintf(fout, "Received: %s\n", recvbuf);
	}
	else if (bytes == 0) // chyba
	{
		if (WSAGetLastError() != WSAETIMEDOUT)
		{
			fprintf(fout, "Connection closed\n");
			return false;
		}
	}
	else  // chyba
	{
		if (WSAGetLastError() != WSAETIMEDOUT)
		{
			fprintf(fout, "recv failed with error: %d\n", WSAGetLastError());
			return false;
		}
	}

	return true;
}

void CppCLRWinformsProjekt::ChatClient::Close()
{
	// cleanup
	is_closed = true;
	closesocket(ConnectSocket);
	WSACleanup();
}

char* CppCLRWinformsProjekt::ChatClient::GetBuffer()
{
	return recvbuf;
}

