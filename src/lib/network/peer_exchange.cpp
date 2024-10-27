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

std::string Handshake::formRequest() const
{
	std::stringstream packet;

	packet << (uint8_t)protocol.length()
		   << protocol;

	packet << bitos(0)
		   << bitos(0);

	packet << hex_to_bin(info_hash)
		   << hex_to_bin(peer_id);

	return packet.str();
}

Handshake Handshake::parse(const std::string &buffer)
{
	std::string_view view(buffer);

	size_t protocolSize = view.front();
	view.remove_prefix(1);

	std::string protocol = std::string(view.substr(0, protocolSize));
	view.remove_prefix(protocolSize + RESERVED_BYTES_SIZE);

	std::string infoHash = std::string(view.substr(0, INFO_HASH_SIZE));
	view.remove_prefix(INFO_HASH_SIZE);

	std::string peerId = std::string(view.substr(0, PEER_ID_SIZE));
	view.remove_prefix(PEER_ID_SIZE);

	Handshake handshake(infoHash, peerId, protocol);
	return handshake;
}

PeerMessage::PeerMessage(PeerMessagesType type, const std::string &payload)
{
	m_message_type = type;
	m_payload = payload;
}

std::string PeerMessage::formMessage() const
{
	std::stringstream packet;
	packet << bitos(m_payload.size() + 1)
		   << (uint8_t)m_message_type
		   << m_payload;
	return packet.str();
}

PeerMessage PeerMessage::parse(const std::string &data)
{
	std::string_view view(data);

	auto size = bstoi(std::string(view.substr(0, sizeof(int))));
	view.remove_prefix(sizeof(int));

	auto type = static_cast<PeerMessagesType>((int)view.front());
	view.remove_prefix(1);

	auto payload = std::string(view);
	view.remove_prefix(payload.size());

	PeerMessage message(type, payload);
	return message;
}

std::string PeerMessage::getPayload() const
{
	return m_payload;
}

PeerMessagesType PeerMessage::getType() const
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