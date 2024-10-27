#include <iostream>
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

IpAddress::IpAddress(const std::string &ip, uint16_t port) : ip(ip), port(port)
{
}

std::ostream &operator<<(std::ostream &out, const IpAddress &address)
{
	out << address.ip << ":" << (int)address.port;
	return out;
}