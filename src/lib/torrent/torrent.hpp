#ifndef TORRENT_READER_H
#define TORRENT_READER_H

#include <vector>

#include <lib/network/network.hpp>
#include <lib/nlohmann/json.hpp>

using json = nlohmann::json;

class Torrent {
private:
	json m_torrent;
	std::vector<std::string> *m_peers = nullptr;

public:
	std::string announce;
	size_t info_length;
	size_t piece_length;
	std::string info_hash;
	std::vector<std::string> piece_hashes;
	size_t interval;

	Torrent(const std::string &path);
	std::vector<std::string> getPeers();
	std::string handshake(const IpAddress &address) const;
};

#endif