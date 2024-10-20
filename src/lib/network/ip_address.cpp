#include <stdexcept>

#include "network.hpp"

IpAddress::IpAddress(const std::string &address)
{
	size_t colonIndex = address.find(':');

	if (colonIndex == std::string::npos)
		throw std::invalid_argument("Provided address doesn't have a colon");

	ip = address.substr(0, colonIndex);
	port = std::stoi(address.substr(colonIndex + 1));
}