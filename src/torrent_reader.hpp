#ifndef TORRENT_READER_H
#define TORRENT_READER_H

#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

json read_torrent_file(const std::string &path);
std::string read_torrent_annouce(const json &torrent);
size_t read_torrent_info_length(const json &torrent);
std::string calc_torrent_info_hash(const json &torrent);

#endif