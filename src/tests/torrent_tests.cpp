#include <vector>

#include "doctest.h"
#include "utils.hpp"
#include <debug_macros.hpp>
#include <lib/torrent/torrent.hpp>

// ========== tests for PeerSocketWrapper ===========================================

TEST_CASE("<PeerSocketWrapper> send")
{
	auto addr = reserve_address();
	int server_sockfd = make_server(addr);
	auto payload = ByteStream("hello world");
	auto client = PeerSocketWrapper(addr);

	auto message = PeerMessage(PeerMessagesType::BITFIELD, payload);
	auto size = message.form_message().size();
	std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
	client.connect();
	client.send(message);
	int conn = accept_client(server_sockfd);

	int total = 0;
	while (total < size)
		total += recv(conn, &buffer[total], size - total, 0);

	ByteStream request(buffer, size);
	CHECK(request == message.form_message());
}

TEST_CASE("<PeerSocketWrapper> recv")
{
	auto addr = reserve_address();
	int server_sockfd = make_server(addr);
	auto payload = ByteStream("hello world");
	auto client = PeerSocketWrapper(addr);

	auto message = PeerMessage(PeerMessagesType::BITFIELD, payload);
	auto sresponse = message.form_message();
	auto size = sresponse.size();
	std::vector<uint8_t> buffer(sresponse.begin(), sresponse.end());
	client.connect();
	int conn = accept_client(server_sockfd);

	int total = 0;
	while (total < size)
		total += send(conn, &buffer[total], size - total, 0);

	auto response = client.recv();
	CHECK(response.form_message() == sresponse);
}

TEST_CASE("<PeerSocketWrapper> send & recv multiple of times")
{
	auto addr = reserve_address();
	int server_sockfd = make_server(addr);
	auto payload = ByteStream("hello world");
	auto client = PeerSocketWrapper(addr);
	client.connect();
	int conn = accept_client(server_sockfd);

	auto message = PeerMessage(PeerMessagesType::BITFIELD, payload);
	auto sresponse = message.form_message();
	auto size = sresponse.size();
	std::vector<uint8_t> buffer(sresponse.begin(), sresponse.end());

	int total = 0;
	while (total < size)
		total += send(conn, &buffer[total], size - total, 0);

	auto response = client.recv();
	CHECK(response.form_message() == sresponse);

	payload = ByteStream("my world is not exactly your world");
	message = PeerMessage(PeerMessagesType::BITFIELD, payload);
	sresponse = message.form_message();
	size = sresponse.size();
	buffer = std::vector<uint8_t>(sresponse.begin(), sresponse.end());

	total = 0;
	while (total < size)
		total += send(conn, &buffer[total], size - total, 0);

	response = client.recv();
	CHECK(response.form_message() == sresponse);
}