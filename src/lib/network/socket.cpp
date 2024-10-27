#include <unistd.h>
#include <vector>

#include "network.hpp"
#include <debug_macros.hpp>
#include <lib/utils/utils.hpp>

SocketClient::SocketClient(IpAddress address)
{
	m_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	m_server_address.sin_family = AF_INET;
	m_server_address.sin_addr.s_addr = inet_addr(address.ip.c_str());
	m_server_address.sin_port = htons(address.port);
}

void SocketClient::connect()
{
	int response = ::connect(m_server_socket_fd, (struct sockaddr *)&m_server_address, sizeof(m_server_address));
	if (response == -1) {
		exit_with_message("Couldn't connect, peer unreachable");
	}
}

void SocketClient::send(const std::string &message) const
{
	std::string_view view(message);
	do {
		ssize_t sent = ::send(m_server_socket_fd, view.data(), view.size(), 0);
		if (sent == -1) {
			close(m_server_socket_fd);
			exit_with_message("Failed to send message");
		}
		view.remove_prefix(sent);
	} while (view.size());
}

std::string SocketClient::recv(int count) const
{
	std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
	ssize_t total = 0;
	do {
		int received = ::recv(m_server_socket_fd, &buffer[total], count - total, 0);
		if (received == -1) {
			close(m_server_socket_fd);
			exit_with_message("Failed to receive message response");
		}
		total += received;
	} while (total < count);

	std::string message(buffer.cbegin(), buffer.cbegin() + count);
	return message;
}

SocketClient::~SocketClient()
{
	close(m_server_socket_fd);
}