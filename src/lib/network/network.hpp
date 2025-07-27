#pragma once
#ifndef NETWORK_H
#define NETWORK_H

#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>

#include <lib/containers/bytestream.hpp>

#define INFO_HASH_SIZE 20
#define PEER_ID_SIZE 20
#define RESERVED_BYTES_SIZE 8
#define MAX_BUFFER_SIZE 32 * 1024

enum class PeerMessagesType {
	CHOKE,
	UNCHOKE,
	INTERESTED,
	NOT_INTERESTED,
	HAVE,
	BITFIELD,
	REQUEST,
	PIECE,
	CANCEL,
};

// [IpAddress]

class IpAddress {
private:
public:
	std::string ip;
	uint16_t port;

	IpAddress(const std::string &address);
	IpAddress(const std::string &ip, uint16_t port);
};

std::ostream &operator<<(std::ostream &out, const IpAddress &address);

// [Handshake]

class Handshake {
private:
public:
	std::string protocol;
	std::string info_hash;
	std::string peer_id;

	Handshake(const std::string &infoHash, const std::string &peerId, const std::string &protocol = "BitTorrent protocol");
	ByteStream form_request() const;

	static Handshake parse(ByteStream buffer);
};

// [PeerMessage]

class PeerMessage {
private:
	PeerMessagesType m_message_type;
	ByteStream m_payload;

public:
	PeerMessage(PeerMessagesType type);
	PeerMessage(PeerMessagesType type, const ByteStream &payload);
	ByteStream form_message() const;
	ByteStream get_payload() const;
	PeerMessagesType get_type() const;
	bool isType(const PeerMessagesType &type) const;
	ssize_t size() const;

	static PeerMessage parse(ByteStream data);
};

class SocketClient {
private:
	int m_server_socket_fd;
	sockaddr_in m_server_address;

public:
	SocketClient(IpAddress address);
	void connect();
	void send(const ByteStream &data) const;
	ByteStream recv(int count) const;
	~SocketClient();
};

#endif