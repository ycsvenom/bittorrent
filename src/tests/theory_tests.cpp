#include <iterator>
#include <sstream>
#include <stdint.h>
#include <string>

#include "doctest.h"
#include <debug_macros.hpp>
#include <lib/utils/utils.hpp>

TEST_CASE("<THEORY> test string related theories")
{
	std::stringstream ss;
	ss << "hello" << (uint8_t)0 << (uint8_t)0 << " world";

	SUBCASE("test length of null bytes in stringstream")
	{
		CHECK(ss.str().length() == 13);
	}
}

TEST_CASE("<THEORY> multiple no-terminator strings to ss")
{
	std::stringstream ss;
	ss << bitos(0)
	   << bitos(0)
	   << bitos(12000);
	std::string out = ss.str();
	CHECK(out.size() == 12);
}

TEST_CASE("<THEORY> string_view size is actual size")
{
	std::stringstream ss;
	ss << bitos(0)
	   << bitos(0)
	   << bitos(12000);
	std::string out = ss.str();
	CHECK(out.size() == std::string_view(out).size());
}