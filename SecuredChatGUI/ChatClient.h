#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <Windows.h>
#include <WS2tcpip.h>
#include <stdio.h>


namespace CppCLRWinformsProjekt
{

#define DEFAULT_BUFLEN 5120
#define DEFAULT_PORT "27015"

	ref class ChatClient
	{
	public:
		bool Init(const char* host); // connect to server
		bool Send(const char* msg);
		bool Receive();
		void Close();
		char* GetBuffer();
		bool is_closed = false;

	private:
		SOCKET ConnectSocket; // SOCKET == unsingned int
		char *recvbuf;
		int buflen = DEFAULT_BUFLEN;
		FILE* fout; // debugovani
	};
}
