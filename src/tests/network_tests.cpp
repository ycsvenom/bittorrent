#include <sstream>

#include "doctest.h"
#include <debug_macros.hpp>
#include <lib/network/network.hpp>

// ========== tests for IpAddress =============================================

TEST_CASE("testing constructor")
{
	std::string ip = "127.0.0.1";
	u_int16_t port = 443;
	std::stringstream addressStream;

	SUBCASE("valid")
	{
		addressStream << ip << ':' << port;
		std::string address = addressStream.str();
		IpAddress addr(address);

		CHECK(addr.ip == ip);
		CHECK(addr.port == port);
	}

	SUBCASE("invalid")
	{
		addressStream << ip << port;
		std::string address = addressStream.str();

		CHECK_THROWS_AS(IpAddress addr(address), std::invalid_argument);
	}
}