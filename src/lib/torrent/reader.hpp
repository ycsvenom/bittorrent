#ifndef TORRENT_READER_H
#define TORRENT_READER_H

#include <vector>

#include <lib/nlohmann/json.hpp>

using json = nlohmann::json;

class Torrent {
private:
	json m_torrent;

public:
	Torrent(const std::string &path);
	std::string announce;
	size_t info_length;
	size_t piece_length;
	std::string info_hash;
	std::vector<std::string> piece_hashes;
};

#endif