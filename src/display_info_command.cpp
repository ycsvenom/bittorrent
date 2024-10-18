#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lib/bencode/bencode.hpp"
#include "display_info_command.hpp"
#include "torrent_reader.hpp"
#include "utils.hpp"

std::stringstream get_display_info_help(const std::string &name)
{
	std::stringstream ss;
	ss << "Usage: " << name << " info <torrent_path>";
	return ss;
}

void display_info_command(int argc, char *argv[])
{
	if (argc < 3) {
		exit_with_message(get_display_info_help(argv[0]));
	}

	std::string path = argv[2];
	json torrent = read_torrent_file(path);
	std::string announce = read_torrent_annouce(torrent);
	size_t infoLength = read_torrent_info_length(torrent);
	std::string infoHash = calc_torrent_info_hash(torrent);

	std::cout << "Tracker URL: " << announce << std::endl;
	std::cout << "Length: " << infoLength << std::endl;
	std::cout << "Info Hash: " << infoHash << std::endl;
}