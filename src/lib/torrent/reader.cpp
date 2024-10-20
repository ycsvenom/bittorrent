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
#define BYTES_PER_PEER 6

json listen_to_announcer(const std::string &, const std::string &, const size_t &);

Torrent::Torrent(const std::string &path)
{
	std::ifstream file(path, std::ios_base::in);
	std::stringstream ss;
	ss << file.rdbuf();

	m_torrent = decode_bencoded_value(ss.str());
	SHA1 sha1;
	json info = m_torrent["info"];
	std::string pieces = info["pieces"];
	size_t segmentsCount = pieces.length() / sha1.HashBytes;

	for (size_t i = 0; i < segmentsCount; i++) {
		auto start = i * sha1.HashBytes;
		auto binPiece = pieces.substr(start, sha1.HashBytes);
		piece_hashes.push_back(bin_to_hex(binPiece));
	}

	announce = m_torrent["announce"];
	info_length = info["length"];
	piece_length = info["piece length"];
	info_hash = sha1(encode_json_to_bencode(info));
}

std::vector<std::string> Torrent::getPeers()
{
	if (m_peers != nullptr)
		return *m_peers;

	m_peers = new std::vector<std::string>();
	json announcement = listen_to_announcer(announce, info_hash, piece_length);
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
		temp << (int)o1 << '.' << (int)o2 << '.' << (int)o3 << '.' << (int)o4 << ':' << port;
		m_peers->push_back(temp.str());
	}

	return *m_peers;
}

json listen_to_announcer(const std::string &announce, const std::string &infoHash, const size_t &pieceLength)
{
	std::vector<std::string> peers;
	std::stringstream urlStream;

	std::string info_hash = url_encode(hex_to_bin(infoHash));
	std::string peer_id = gen_hash(20);
	short port = 6881;
	size_t uploaded = 0;
	size_t downloaded = 0;
	size_t left = pieceLength;
	bool compact = true;

	urlStream << announce << "?"
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