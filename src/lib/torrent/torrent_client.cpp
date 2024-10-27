#include <fstream>
#include <iomanip>

#include "torrent.hpp"
#include <debug_macros.hpp>
#include <lib/bencode/bencode.hpp>
#include <lib/elnormous/HTTPRequest.hpp>
#include <lib/hash/sha1.h>
#include <lib/utils/utils.hpp>

#define FIRST_GET_ARG(arg) #arg << "=" << arg
#define GET_ARG(arg) "&" << FIRST_GET_ARG(arg)
#define FORM_IP(o1, o2, o3, o4) (int)o1 << '.' << (int)o2 << '.' << (int)o3 << '.' << (int)o4

#define BYTES_PER_PEER 6
#define HANDSHAKE_SIZE 68

json listen_to_announcer(const TorrentFile &);

std::string make_message(const PeerMessage &message)
{
	std::stringstream error;
	error << "incorrect message type "
		  << (int)message.getType()
		  << "[!]";
	return error.str();
}

TorrentClient::TorrentClient(const std::string &path) : m_torrent(path)
{
}

TorrentClient::TorrentClient(const TorrentFile &torrent) : m_torrent(torrent)
{
}

std::vector<IpAddress> TorrentClient::getPeers()
{
	if (m_peers != nullptr)
		return *m_peers;

	m_peers = new std::vector<IpAddress>();
	json announcement = listen_to_announcer(m_torrent);
	interval = announcement["interval"];
	std::string binPeers = announcement["peers"];

	for (size_t i = 0; i < binPeers.length(); i += BYTES_PER_PEER) {
		uint8_t o1 = static_cast<uint8_t>(binPeers[i]);
		uint8_t o2 = static_cast<uint8_t>(binPeers[i + 1]);
		uint8_t o3 = static_cast<uint8_t>(binPeers[i + 2]);
		uint8_t o4 = static_cast<uint8_t>(binPeers[i + 3]);
		uint8_t p1 = static_cast<uint8_t>(binPeers[i + 4]);
		uint8_t p2 = static_cast<uint8_t>(binPeers[i + 5]);
		uint16_t port = (static_cast<uint16_t>(p1) << 8) | static_cast<uint16_t>(p2);
		std::stringstream temp;
		temp << FORM_IP(o1, o2, o3, o4);
		m_peers->push_back(IpAddress(temp.str(), port));
	}

	return *m_peers;
}

std::string TorrentClient::handshake(const IpAddress &address)
{
	m_client = new PeerSocketWrapper(address);
	m_client->connect();

	m_peer_id = bin_to_hex(gen_hash(PEER_ID_SIZE));
	Handshake handshake(hex_to_bin(m_torrent.info_hash), hex_to_bin(m_peer_id));
	auto request = handshake.formRequest();
	m_client->send(request);

	auto received = m_client->recv(HANDSHAKE_SIZE);
	Handshake peerHandshake = Handshake::parse(received);

	return peerHandshake.peer_id;
}

void TorrentClient::downloadPiece(const std::string &path, size_t pieceIndex) const
{
	std::ofstream out(path, std::ios_base::out);

	if (!out) {
		std::stringstream error;
		error << "Error creating file: " << path << std::endl;
		exit_with_message(error);
	}

	auto message = m_client->recv();
	if (!message.isType(PeerMessagesType::BITFIELD))
		exit_with_message(make_message(message));

	m_client->send(PeerMessage(PeerMessagesType::INTERESTED));
	
	message = m_client->recv();
	if (!message.isType(PeerMessagesType::UNCHOKE))
		exit_with_message(make_message(message));

	size_t size = m_torrent.piece_length;
	size_t blockIndex = 0;

	while (size > 0) {
		size_t blockSize = size > BLOCK_SIZE ? BLOCK_SIZE : size;
		std::stringstream ss;
		ss << bitos(pieceIndex)
		   << bitos(BLOCK_SIZE * (blockIndex++))
		   << bitos(blockSize);
		m_client->send(PeerMessage(PeerMessagesType::REQUEST, ss.str()));
		
		message = m_client->recv();
		// if (!message.isType(PeerMessagesType::PIECE))
		// 	exit_with_message(make_message(message));

		auto payload = std::string_view(message.getPayload());

		[[maybe_unused]]
		auto index = bstoi(std::string(payload.substr(0, sizeof(int))));
		payload.remove_prefix(sizeof(int));

		[[maybe_unused]]
		auto begin = bstoi(std::string(payload.substr(0, sizeof(int))));
		payload.remove_prefix(sizeof(int));

		auto block = payload;

		out << block;
		size -= block.size();
	}
}

json listen_to_announcer(const TorrentFile &torrent)
{
	std::vector<std::string> peers;
	std::stringstream urlStream;

	std::string info_hash = url_encode(hex_to_bin(torrent.info_hash));
	std::string peer_id = gen_hash(PEER_ID_SIZE);
	short port = 6881;
	size_t uploaded = 0;
	size_t downloaded = 0;
	size_t left = torrent.piece_length;
	bool compact = true;

	urlStream << torrent.announce << "?"
			  << FIRST_GET_ARG(info_hash)
			  << GET_ARG(peer_id)
			  << GET_ARG(port)
			  << GET_ARG(uploaded)
			  << GET_ARG(downloaded)
			  << GET_ARG(left)
			  << GET_ARG(compact);

	std::string url = urlStream.str();
	http::Request request{url};
	http::Response response = request.send("GET");
	json info = decode_bencoded_value(std::string(response.body.begin(), response.body.end()));
	return info;
}