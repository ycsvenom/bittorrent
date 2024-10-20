#include <iostream>
#include <sstream>
#include <string>

#include "peers.hpp"
#include <lib/torrent/torrent.hpp>
#include <lib/utils/utils.hpp>

std::stringstream get_peers_help(const std::string &name)
{
	std::stringstream ss;
	ss << "Usage: " << name << " peers <torrent_path>";
	return ss;
}

void peers_command(int argc, char *argv[])
{
	if (argc < 3) {
		exit_with_message(get_peers_help(argv[0]));
	}

	std::string path = argv[2];
	Torrent torrent(path);

	for (auto peer : torrent.getPeers()) {
		std::cout << peer << std::endl;
	}
}
