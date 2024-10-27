#include <vector>

#include "doctest.h"
#include "utils.hpp"
#include <lib/torrent/torrent.hpp>

// ========== tests for PeerSocketWrapper ===========================================

TEST_CASE("send")
{
	auto addr = reserve_address();
	int server_sockfd = make_server(addr);
	auto client = PeerSocketWrapper(addr);

	auto message = PeerMessage(PeerMessagesType::BITFIELD, "hello world");
	auto size = message.formMessage().size();
	std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
	client.connect();
	client.send(message);
	int conn = accept_client(server_sockfd);

	int total = 0;
	while (total < size)
		total += recv(conn, &buffer[total], size - total, 0);

	std::string request(buffer.cbegin(), buffer.cbegin() + size);
	CHECK(request == message.formMessage());
}

TEST_CASE("recv")
{
	auto addr = reserve_address();
	int server_sockfd = make_server(addr);
	auto client = PeerSocketWrapper(addr);

	auto message = PeerMessage(PeerMessagesType::BITFIELD, "hello world");
	auto sresponse = message.formMessage();
	auto size = sresponse.size();
	std::vector<uint8_t> buffer(sresponse.begin(), sresponse.end());
	client.connect();
	int conn = accept_client(server_sockfd);

	int total = 0;
	while (total < size)
		total += send(conn, &buffer[total], size - total, 0);

	auto response = client.recv();
	CHECK(response.formMessage() == sresponse);
}