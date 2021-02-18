
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include <map>
#include <iostream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")


#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 27015
#define DEFAULT_PASS "prg3"


#define DEFAULT_BUFLEN 5120
#define MAX_CLIENTS 30


class SocketServer
{
public:
	SocketServer(const char* password) : password(password) {}
	~SocketServer() { delete[] password; delete[] recvbuf; }
	void Init();
	void Loop();
	void Close();
private:
	WSADATA wsaData;
	const char* password;
	int i_ok; // error status

	// SOCKET is typedef for unsigned integer
	SOCKET m_listening = INVALID_SOCKET;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	struct sockaddr_in client;
	// set of socket descriptors
	fd_set m_master;	
	// highest socket descriptor, need it for the select() function
};

std::string SocketToName(SOCKET sock)
{
	std::ostringstream ss;
	ss << sock;
	return ss.str();
}


void SocketServer::Init()
{
	// Initialize Winsock
	WORD ver = MAKEWORD(2, 2);
	i_ok = WSAStartup(ver, &wsaData);
	if (i_ok != 0) {
		printf("[ERROR] WSAStartup failed with error: %d\n", i_ok);
		exit(1);
	}

	// Create a SOCKET for connecting to server
	m_listening = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listening == INVALID_SOCKET) {
		printf("[ERROR] socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		exit(1);
	}

	// Setup the TCP listening socket and bind it to the port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(DEFAULT_PORT);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	i_ok = bind(m_listening, (sockaddr*)&hint, sizeof(hint));
	if (i_ok == SOCKET_ERROR) {
		printf("[ERROR] bind failed with error: %d\n", WSAGetLastError());
		closesocket(m_listening);
		WSACleanup();
		exit(1);
	}

	// Tell Winsock the socket is for listening
	// SOMAXCONN is maximum number of pending connections
	i_ok = listen(m_listening, SOMAXCONN);
	if (i_ok == SOCKET_ERROR) {
		printf("[ERROR] listen failed with error: %d\n", WSAGetLastError());
		closesocket(m_listening);
		WSACleanup();
		exit(1);
	}
}

void SocketServer::Loop()
{
	// Create the master file descriptor set and zero it
	// Clear the socket set
	FD_ZERO(&m_master);
	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added to our server or else we won't hear incomming connections
	FD_SET(m_listening, &m_master);

	bool running = true;
	while (running)
	{
		// Make a copy because the call to select() is destructive.
		fd_set copy = m_master;// mnozina socketu
		int socketCount = select(0, &copy, NULL, NULL, NULL);
		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copy.fd_array[i];
			if (sock == m_listening)
			{
				// Accept a new connection
				SOCKET client = accept(m_listening, NULL, NULL);
				// Add the new connection to the list of connected
				FD_SET(client, &m_master);
				// Send a welcome message to the connected client
				// send(client, WelcomeMessage.c_str(), WelcomeMessage.size() + 1, 0);
				printf("[INFO] (%d) New connection accepted\n", client);
				// printf("[INFO] (%d) Welcome message was sent\n", client);
				char keybuf[DEFAULT_BUFLEN];
				int buflen = DEFAULT_BUFLEN;
				ZeroMemory(keybuf, buflen);
				int bytes = recv(client, keybuf, buflen, 0);
				if (bytes > 0)
				{
					if (strcmp(keybuf, password) != 0)
					{
						printf("[INFO] (%d) Password Incorrect: %s. Disconnect\n", client, keybuf);
						closesocket(client);
						FD_CLR(client, &m_master);
					}
					else
					{
						printf("[INFO] (%d) Welcome Password: %s\n", client, keybuf);
					}
				}
			}
			else
			{
				// Receive message from socket
				ZeroMemory(recvbuf, recvbuflen);
				int bytesIn = recv(sock, recvbuf, recvbuflen, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					// printf("[INFO] (%d) Connection error, but let to stay\n", sock);
					printf("[INFO] (%d) Connection closed\n", sock);
					closesocket(sock);
					FD_CLR(sock, &m_master);
				}
				else if (bytesIn > 0)
				{
					// Send message to other clients
					for (int i = 0; i < m_master.fd_count; i++)
					{
						SOCKET outSock = m_master.fd_array[i];
						// Other clients are neither listeners nor the sender socket
						if (outSock != m_listening && outSock != sock)
						{
							std::ostringstream ss;
							ss << recvbuf;
							std::string strOut = ss.str();
							send(outSock, recvbuf, recvbuflen, 0);

							printf("[INFO] (%d) Message '%s' broadcasted to %d\n", sock, recvbuf, outSock);

						}
					}
				}
			}
		}
	}
}


void SocketServer::Close()
{
	printf("[INFO] Closing\n");

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(m_listening, &m_master);
	closesocket(m_listening);

	// Message to let users know what's happening.
	std::string msg = "Server is shutting down. Goodbye\r\n";

	while (m_master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = m_master.fd_array[0];

		// Send the goodbye message
		send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &m_master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
}

int main(int argc, char* argv[])
{
	std::string heslo;
	if (argc == 2)
	{
		heslo = std::string(argv[1]);
	}
	else
	{
		heslo = DEFAULT_PASS;
	}

	SocketServer server(heslo.c_str());
	printf("[INFO] Starting the server on %s:%d\n", DEFAULT_HOST, DEFAULT_PORT);
	printf("[INFO] password is '%s'\n", heslo.c_str());
	server.Init();
	printf("[INFO] Listening for new connections\n");
	server.Loop();
	//printf("[INFO] Closing the server\n");
	server.Close();
	return 0;
}
