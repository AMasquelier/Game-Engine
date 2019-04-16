#pragma once
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#pragma comment(lib, "ws2_32.lib")

//  /!\ Work in progress /!\  \\

class Network
{
	public:
		Network();
		void Connect();
		void Disconnect();
		int Send(const char *buffer, int len);
		int Recv(char *buffer, int len);
		int send_file(const char *filename);
		int recv_file(const char *filename);

		bool is_connected();

	private:
		WSADATA WSAData;
		SOCKET sock;
		SOCKADDR_IN sin;

		bool _connected = false;
};