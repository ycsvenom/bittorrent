#include "torrent.hpp"
#include <debug_macros.hpp>
#include <lib/utils/utils.hpp>

void PeerSocketWrapper::send(const PeerMessage &message) const
{
	auto data = message.formMessage();
	send(data);
}

PeerMessage PeerSocketWrapper::recv() const
{
	size_t size = 0;
	do {
		size = bstoi(recv(4));
	} while (!size);
	auto body = std::string_view(recv(size));
	auto type = static_cast<PeerMessagesType>(body.front());
	body.remove_prefix(1);
	std::string payload = std::string(body);
	auto message = PeerMessage(type, payload);
	return message;
}