#include "Network.h"

Network::Network()
{
}

void Network::Connect()
{
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(42);

	int connected = connect(sock, (SOCKADDR *)&sin, sizeof(sin));
	_connected = (connected == 0);
}

void Network::Disconnect()
{
	closesocket(sock);
	WSACleanup();
}

int Network::Send(const char * buffer, int len)
{
	return send(sock, buffer, len, 0);
}

int Network::Recv(char * buffer, int len)
{
	return recv(sock, buffer, len, 0);;
}

int Network::send_file(const char * filename)
{
	struct stat results;
	stat(filename, &results);
	long size = results.st_size;
	char * buffer = nullptr;

	// TODO //

	send(sock, (char*)(&size), sizeof(long), 0);
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		buffer = new char[2048];
		file.seekg(0, std::ios::beg);
		int pos = 0;
		int sent = 256;
		while (pos < size)
		{
			file.read(buffer, 2048);
			sent = send(sock, buffer, 2048 - max(2048 - (size - pos), 0), 0);

			pos += sent;
		}
		file.close();
	}
	else std::cout << "Unable to open file";
}

int Network::recv_file(const char *filename)
{
	char *buffer;
	char buf[4];

	// TODO //

	recv(sock, buf, sizeof(int), 0);
	long size = *(long*)(buf);
	buffer = new char[2048];

	std::ofstream file(filename, std::ios::out | std::ios::binary);

	int pos = 0;
	while (pos < size)
	{
		int get = recv(sock, buffer, 2048, 0);
		file.write(buffer, get);
		pos += get;
	}

	file.close();

	struct stat results;
	stat(filename, &results);

	return results.st_size;
}

bool Network::is_connected()
{
	return _connected;
}
