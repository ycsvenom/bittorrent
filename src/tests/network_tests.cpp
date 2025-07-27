#include <sstream>
#include <vector>

#include "doctest.h"
#include "utils.hpp"
#include <debug_macros.hpp>
#include <lib/network/network.hpp>
#include <lib/utils/utils.hpp>

// ========== tests for IpAddress =============================================

TEST_CASE("<IpAddress> constructor")
{
	std::string ip = "127.0.0.1";
	uint16_t port = 443;
	ByteStream addressStream;

	SUBCASE("valid")
	{
		addressStream << ip << ':' << std::to_string(port);
		std::string address = addressStream.pop_str();
		IpAddress addr(address);

		CHECK(addr.ip == ip);
		CHECK(addr.port == port);
	}

	SUBCASE("invalid")
	{
		addressStream << ip << port;
		std::string address = addressStream.pop_str();

		CHECK_THROWS_AS(IpAddress addr(address), std::invalid_argument);
	}
}

TEST_CASE("<IpAddress> stream insertion operator")
{
	auto address = IpAddress("192.168.0.0", 443);
	std::stringstream ss;
	ss << address;
	CHECK(ss.str() == "192.168.0.0:443");
}

// ========== tests for Handshake =============================================

TEST_CASE("<Handshake> form_request & parse")
{
	std::string protocol = "hello";
	std::string infoHash = "01234567890123456789";
	std::string peerId = "12345678901234567890";
	ByteStream stream;

	/*
		packet = [protocol_size]
				 [protocol]
				 [8 zeros reserved bytes]
				 [info_hash]
				 [peed_id]
	*/
	stream << (uint8_t)protocol.size()
		   << protocol
		   << (uint8_t)0
		   << (uint8_t)0
		   << (uint8_t)0
		   << (uint8_t)0
		   << (uint8_t)0
		   << (uint8_t)0
		   << (uint8_t)0
		   << (uint8_t)0
		   << infoHash
		   << peerId;

	SUBCASE("parse")
	{
		auto handshake = Handshake::parse(stream);
		CHECK(hex_to_bin(handshake.info_hash) == infoHash);
		CHECK(hex_to_bin(handshake.peer_id) == peerId);
		CHECK(handshake.protocol == protocol);
	}

	SUBCASE("form_request")
	{
		auto handshake = Handshake(infoHash, peerId, protocol);
		CHECK(handshake.form_request() == stream);
	}
}

TEST_CASE("<Handshake> peerId and infoHash should must equal to their size")
{
	std::string protocol = "hello";
	std::string infoHash = "01234567890123456789";
	std::string peerId = "12345678901234567890";
	CHECK_THROWS_AS(Handshake(infoHash + "0", peerId, protocol), std::invalid_argument);
	CHECK_THROWS_AS(Handshake(infoHash, peerId + "0", protocol), std::invalid_argument);
}

// ========== tests for PeerMessage ===========================================

TEST_CASE("<PeerMessage> form_message & parse")
{
	auto type = PeerMessagesType::BITFIELD;
	ByteStream payload("01234567890123456789");
	ByteStream stream;

	/*
		packet = [message size]
				 [type]
				 [payload]
	*/
	stream << payload.size() + 1
		   << (uint8_t)type
		   << payload;

	SUBCASE("parse")
	{
		auto message = PeerMessage::parse(stream);
		CHECK(message.get_type() == type);
		CHECK(message.get_payload() == payload);
	}

	SUBCASE("form_message")
	{
		auto message = PeerMessage(type, payload);
		CHECK(message.form_message() == stream);
	}
}

TEST_CASE("<PeerMessage> is_type")
{
	auto message = PeerMessage(PeerMessagesType::BITFIELD);
	CHECK(message.isType(PeerMessagesType::BITFIELD));
}

// ========== tests for SocketClient ===========================================

TEST_CASE("<SocketClient> send")
{
	auto addr = reserve_address();
	int server_sockfd = make_server(addr);
	auto client = SocketClient(addr);

	auto size = 2048;
	auto text = ByteStream(std::string(size, '0'));
	std::vector<uint8_t> buffer(size);
	client.connect();
	client.send(text);
	int conn = accept_client(server_sockfd);

	int total = 0;
	while (total < size)
		total += recv(conn, &buffer[total], size - total, 0);

	ByteStream message(buffer, size);
	CHECK(message == text);
}

TEST_CASE("<SocketClient> recv")
{
	auto addr = reserve_address();
	int server_sockfd = make_server(addr);
	auto client = SocketClient(addr);

	auto size = MAX_BUFFER_SIZE;
	auto text = std::string(size, '0');
	std::vector<uint8_t> buffer(text.begin(), text.end());
	client.connect();
	int conn = accept_client(server_sockfd);

	int total = 0;
	while (total < size)
		total += send(conn, &buffer[total], size - total, 0);

	auto response = client.recv(size);
	CHECK(response == text);
}