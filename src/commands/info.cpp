#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "info.hpp"
#include <lib/bencode/bencode.hpp>
#include <lib/torrent/torrent.hpp>
#include <lib/utils/utils.hpp>

std::stringstream get_info_help(const std::string &name)
{
	std::stringstream ss;
	ss << "Usage: " << name << " info <torrent_path>";
	return ss;
}

void info_command(int argc, char *argv[])
{
	if (argc < 3) {
		exit_with_message(get_info_help(argv[0]));
	}

	std::string path = argv[2];
	Torrent torrent(path);

	std::cout << "Tracker URL: " << torrent.announce << std::endl;
	std::cout << "Length: " << torrent.info_length << std::endl;
	std::cout << "Info Hash: " << torrent.info_hash << std::endl;
	std::cout << "Piece Length: " << torrent.piece_length << std::endl;
	std::cout << "Piece Hashes:" << std::endl;
	for (auto hash : torrent.piece_hashes) {
		std::cout << hash << std::endl;
	}
}