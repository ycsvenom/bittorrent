#include <sstream>
#include <stdint.h>
#include <string_view>

#include "network.hpp"
#include <debug_macros.hpp>
#include <lib/utils/utils.hpp>

Handshake::Handshake(const std::string &infoHash, const std::string &peerId, const std::string &protocol) : protocol(protocol)
{
	if (infoHash.size() != INFO_HASH_SIZE)
		throw std::invalid_argument("invalid info hash size");

	if (peerId.size() != PEER_ID_SIZE)
		throw std::invalid_argument("invalid peer id size");

	info_hash = bin_to_hex(infoHash);
	peer_id = bin_to_hex(peerId);
}

ByteStream Handshake::form_request() const
{
	ByteStream packet;

	packet << (uint8_t)protocol.length()
		   << protocol;

	packet << 0
		   << 0;

	packet << hex_to_bin(info_hash)
		   << hex_to_bin(peer_id);

	return packet;
}

Handshake Handshake::parse(ByteStream buffer)
{
	size_t protocolSize = buffer.pop_byte();

	std::string protocol = buffer.pop_str(protocolSize);
	buffer.remove_prefix(RESERVED_BYTES_SIZE);

	std::string infoHash = buffer.pop_str(INFO_HASH_SIZE);
	std::string peerId = buffer.pop_str(PEER_ID_SIZE);

	Handshake handshake(infoHash, peerId, protocol);
	return handshake;
}

PeerMessage::PeerMessage(PeerMessagesType type)
{
	m_message_type = type;
}

PeerMessage::PeerMessage(PeerMessagesType type, const ByteStream &payload)
{
	m_message_type = type;
	m_payload = payload;
}

ByteStream PeerMessage::form_message() const
{
	ByteStream packet;
	packet << m_payload.size() + 1
		   << (uint8_t)m_message_type
		   << m_payload;
	return packet;
}

PeerMessage PeerMessage::parse(ByteStream data)
{
	auto size = data.pop_int();

	auto type = static_cast<PeerMessagesType>(data.front());
	data.remove_prefix(1);

	PeerMessage message(type, data);
	return message;
}

ByteStream PeerMessage::get_payload() const
{
	return m_payload;
}

PeerMessagesType PeerMessage::get_type() const
{
	return m_message_type;
}

bool PeerMessage::isType(const PeerMessagesType &type) const
{
	return m_message_type == type;
}

ssize_t PeerMessage::size() const
{
	return m_payload.size();
}
