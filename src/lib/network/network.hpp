#ifndef NETWORK_H
#define NETWORK_H

#include <string>

#define PEER_ID_SIZE 20

class IpAddress {
private:
public:
	std::string ip;
	u_int16_t port;

	IpAddress(const std::string &address);
};

class Handshake {
private:
	std::string m_protocol;
	std::string m_info_hash;

public:
	std::string peer_id;

	Handshake(const std::string &infoHash, const std::string &peerId, const std::string &protocol = "BitTorrent protocol");
	std::string formRequest() const;

	static Handshake parse(const unsigned char *buffer, size_t size);
};

#endif