#include "utils.hpp"

static int port = 18000;

int reserve_port()
{
	return port++;
}

IpAddress reserve_address()
{
	return IpAddress("0.0.0.0", reserve_port());
}

int make_server(IpAddress addr)
{
	int server_sockfd;
	auto host = addr.ip;
	int port = addr.port;
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(host.c_str());
	server_addr.sin_port = htons(port);

	server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	listen(server_sockfd, 20);

	return server_sockfd;
}

int accept_client(int server_sockfd)
{
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	int conn = accept(server_sockfd, (struct sockaddr *)&client_addr, &length);
	return conn;
}