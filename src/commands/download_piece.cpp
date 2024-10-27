#include <iostream>
#include <sstream>
#include <string>

#include "download_piece.hpp"
#include <debug_macros.hpp>
#include <lib/network/network.hpp>
#include <lib/torrent/torrent.hpp>
#include <lib/utils/utils.hpp>

std::stringstream get_download_piece_help(const std::string &name)
{
	std::stringstream ss;
	ss << "Usage: " << name << " download_piece -o <piece_path> <torrent_path> <piece_index>";
	return ss;
}

void download_piece_command(int argc, char *argv[])
{
	if (argc < 6) {
		exit_with_message(get_download_piece_help(argv[0]));
	}

	auto outputPath = argv[3];
	auto torrentPath = argv[4];
	auto pieceIndex = std::stoi(argv[5]);

	TorrentClient torrent(torrentPath);
	auto peers = torrent.getPeers();
	auto peer = peers.front();
	IpAddress address(peer);
	std::string peerId = torrent.handshake(address);
	torrent.downloadPiece(outputPath, pieceIndex);
}
