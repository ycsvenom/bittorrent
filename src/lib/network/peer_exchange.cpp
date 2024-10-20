#include <sstream>
#include <stdint.h>

#include "network.hpp"
#include <lib/utils/utils.hpp>

Handshake::Handshake(const std::string &infoHash, const std::string &peerId, const std::string &protocol)
{
	m_protocol = protocol;
	m_info_hash = infoHash;
	peer_id = peerId;
}

std::string Handshake::formRequest() const
{
	std::stringstream packet;

	packet << (uint8_t)m_protocol.length()
		   << m_protocol;

	for (size_t i = 0; i < 8; i++)
		packet << (uint8_t)0;

	packet << hex_to_bin(m_info_hash)
		   << peer_id;

	return packet.str();
}

Handshake Handshake::parse(const unsigned char *buffer, size_t size)
{
	std::stringstream temp;
	for (size_t i = size - PEER_ID_SIZE; i < size; i++)
		temp << buffer[i];
	Handshake handshake("", bin_to_hex(temp.str()));
	return handshake;
}