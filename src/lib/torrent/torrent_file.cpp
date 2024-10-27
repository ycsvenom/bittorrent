#include <fstream>
#include <iomanip>

#include "torrent.hpp"
#include <debug_macros.hpp>
#include <lib/bencode/bencode.hpp>
#include <lib/elnormous/HTTPRequest.hpp>
#include <lib/hash/sha1.h>
#include <lib/utils/utils.hpp>

TorrentFile::TorrentFile(const std::string &path)
{
	std::ifstream torrentFile(path, std::ios_base::in);

	if (!torrentFile) {
		std::stringstream error;
		error << "Error opening torrent file: " << path << std::endl;
		exit_with_message(error);
	}

	std::stringstream content;
	content << torrentFile.rdbuf();

	m_torrent = decode_bencoded_value(content.str());
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