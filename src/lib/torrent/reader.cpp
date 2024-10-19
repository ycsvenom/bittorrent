#include <fstream>
#include <iomanip>

#include "reader.hpp"
#include <debug_macros.hpp>
#include <lib/bencode/bencode.hpp>
#include <lib/hash/sha1.h>
#include <lib/utils/utils.hpp>

Torrent::Torrent(const std::string &path)
{
	std::ifstream file(path, std::ios_base::in);
	std::stringstream ss;
	ss << file.rdbuf();

	m_torrent = decode_bencoded_value(ss.str());
	SHA1 sha1;
	json info = m_torrent["info"];
	std::vector<std::string> hashes;
	std::string pieces = info["pieces"];
	size_t segmentsCount = pieces.length() / sha1.HashBytes;

	for (size_t i = 0; i < segmentsCount; i++) {
		auto start = i * sha1.HashBytes;
		auto binPiece = pieces.substr(start, sha1.HashBytes);
		hashes.push_back(binary_string_to_hex(binPiece));
	}

	announce = m_torrent["announce"];
	info_length = info["length"];
	piece_length = info["piece length"];
	info_hash = sha1(encode_json_to_bencode(info));
	piece_hashes = hashes;
}