#include "torrent.hpp"
#include <debug_macros.hpp>
#include <lib/utils/utils.hpp>

void PeerSocketWrapper::send(const PeerMessage &message) const
{
	auto data = message.form_message();
	send(data);
}

PeerMessage PeerSocketWrapper::recv() const
{
	size_t size = 0;
	do {
		size = recv(4).pop_uint();
	} while (!size);
	auto stream = recv(size);
	auto type = static_cast<PeerMessagesType>(stream.pop_byte());
	auto payload = stream;
	auto message = PeerMessage(type, payload);
	return message;
}