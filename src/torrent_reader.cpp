#include <fstream>

#include "lib/bencode/bencode.hpp"
#include "lib/hash/sha1.h"
#include "torrent_reader.hpp"

json read_torrent_file(const std::string &path)
{
	std::ifstream file(path, std::ios_base::in);
	std::stringstream ss;
	ss << file.rdbuf();

	json torrent = decode_bencoded_value(ss.str());
	return torrent;
}

std::string read_torrent_annouce(const json &torrent)
{
	json announce = torrent["announce"];
	return announce.get<std::string>();
}

size_t read_torrent_info_length(const json &torrent)
{
	json info = torrent["info"];
	json infoLength = info["length"];
	return infoLength.get<size_t>();
}

std::string calc_torrent_info_hash(const json &torrent)
{
	SHA1 sha1;
	json info = torrent["info"];
	std::string bencoded = encode_json_to_bencode(info);
	std::string hash = sha1(bencoded);
	return hash;
}