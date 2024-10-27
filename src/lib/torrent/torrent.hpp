#pragma once
#ifndef TORRENT_READER_H
#define TORRENT_READER_H

#include <vector>

#include <lib/network/network.hpp>
#include <lib/nlohmann/json.hpp>

#define BLOCK_SIZE 16 * 1024

using json = nlohmann::json;

class TorrentFile {
private:
	json m_torrent;

public:
	std::string announce;
	size_t info_length;
	size_t piece_length;
	std::string info_hash;
	std::vector<std::string> piece_hashes;

	TorrentFile(const std::string &path);
};

class PeerSocketWrapper : public SocketClient {
private:
	size_t m_size;
	PeerMessagesType m_type;
	std::string m_payload;

public:
	using SocketClient::recv;
	using SocketClient::send;
	using SocketClient::SocketClient;
	void send(const PeerMessage &message) const;
	PeerMessage recv() const;
};

class TorrentClient {
private:
	TorrentFile m_torrent;
	PeerSocketWrapper *m_client = nullptr;
	std::vector<IpAddress> *m_peers = nullptr;
	std::string m_peer_id;

public:
	size_t interval;

	TorrentClient(const std::string &path);
	TorrentClient(const TorrentFile &torrent);
	std::vector<IpAddress> getPeers();
	std::string handshake(const IpAddress &address);
	void downloadPiece(const std::string &path, size_t pieceIndex) const;
};

#endif