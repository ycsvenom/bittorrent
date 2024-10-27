#include <sstream>
#include <vector>

#include "doctest.h"
#include "utils.hpp"
#include <debug_macros.hpp>
#include <lib/network/network.hpp>
#include <lib/utils/utils.hpp>

// ========== tests for IpAddress =============================================

TEST_CASE("testing constructor")
{
	std::string ip = "127.0.0.1";
	uint16_t port = 443;
	std::stringstream addressStream;

	SUBCASE("valid")
	{
		addressStream << ip << ':' << port;
		std::string address = addressStream.str();
		IpAddress addr(address);

		CHECK(addr.ip == ip);
		CHECK(addr.port == port);
	}

	SUBCASE("invalid")
	{
		addressStream << ip << port;
		std::string address = addressStream.str();

		CHECK_THROWS_AS(IpAddress addr(address), std::invalid_argument);
	}
}

TEST_CASE("testing stream insertion operator")
{
	auto address = IpAddress("192.168.0.0", 443);
	std::stringstream ss;
	ss << address;
	CHECK(ss.str() == "192.168.0.0:443");
}

// ========== tests for Handshake =============================================

TEST_CASE("testing formRequest & parse")
{
	std::string protocol = "hello";
	std::string infoHash = "01234567890123456789";
	std::string peerId = "12345678901234567890";
	std::stringstream ss;

	/*
		packet = [protocol_size]
				 [protocol]
				 [8 zeros reserved bytes]
				 [info_hash]
				 [peed_id]
	*/
	ss << (uint8_t)protocol.size()
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
		auto handshake = Handshake::parse(ss.str());
		CHECK(hex_to_bin(handshake.info_hash) == infoHash);
		CHECK(hex_to_bin(handshake.peer_id) == peerId);
		CHECK(handshake.protocol == protocol);
	}

	SUBCASE("formRequest")
	{
		auto handshake = Handshake(infoHash, peerId, protocol);
		CHECK(handshake.formRequest() == ss.str());
	}
}

TEST_CASE("peerId and infoHash should must equal to their size")
{
	std::string protocol = "hello";
	std::string infoHash = "01234567890123456789";
	std::string peerId = "12345678901234567890";
	CHECK_THROWS_AS(Handshake(infoHash + "0", peerId, protocol), std::invalid_argument);
	CHECK_THROWS_AS(Handshake(infoHash, peerId + "0", protocol), std::invalid_argument);
}

// ========== tests for PeerMessage ===========================================

TEST_CASE("formMessage & parse")
{
	auto type = PeerMessagesType::BITFIELD;
	std::string payload = "01234567890123456789";
	std::stringstream ss;

	/*
		packet = [message size]
				 [type]
				 [payload]
	*/
	ss << bitos(payload.size() + 1)
	   << (uint8_t)type
	   << payload;

	SUBCASE("parse")
	{
		auto message = PeerMessage::parse(ss.str());
		CHECK(message.getType() == type);
		CHECK(message.getPayload() == payload);
	}

	SUBCASE("formMessage")
	{
		auto message = PeerMessage(type, payload);
		CHECK(message.formMessage() == ss.str());
	}
}

TEST_CASE("testing isType")
{
	auto message = PeerMessage(PeerMessagesType::BITFIELD);
	CHECK(message.isType(PeerMessagesType::BITFIELD));
}

// ========== tests for SocketClient ===========================================

TEST_CASE("send")
{
	auto addr = reserve_address();
	int server_sockfd = make_server(addr);
	auto client = SocketClient(addr);

	auto size = MAX_BUFFER_SIZE;
	auto text = std::string(size, '0');
	std::vector<uint8_t> buffer(size);
	client.connect();
	client.send(text);
	int conn = accept_client(server_sockfd);

	int total = 0;
	while (total < size)
		total += recv(conn, &buffer[total], size - total, 0);

	std::string message(buffer.cbegin(), buffer.cbegin() + size);
	CHECK(message == text);
}

TEST_CASE("recv")
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