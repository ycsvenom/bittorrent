#include <iostream>
#include <sstream>
#include <string>

#include "handshake.hpp"
#include <lib/network/network.hpp>
#include <lib/torrent/torrent.hpp>
#include <lib/utils/utils.hpp>

std::stringstream get_handshake_help(const std::string &name)
{
	std::stringstream ss;
	ss << "Usage: " << name << " handshake <torrent_path> <peer_address>";
	return ss;
}

void handshake_command(int argc, char *argv[])
{
	if (argc < 4) {
		exit_with_message(get_handshake_help(argv[0]));
	}

	std::string path = argv[2];
	IpAddress address(argv[3]);

	Torrent torrent(path);
	std::string peerId = torrent.handshake(address);

	std::cout << "Peer ID: " << peerId << std::endl;
}
