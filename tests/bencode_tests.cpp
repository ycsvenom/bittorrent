#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "src/bencode.hpp"

TEST_CASE("testing decode_bencoded_value")
{
	CHECK(decode_bencoded_value("5:hello").dump() == "\"hello\"");
	CHECK_THROWS_AS(decode_bencoded_value("a:hello").dump(), std::runtime_error);
	CHECK_THROWS_AS(decode_bencoded_value("5hello").dump(), std::runtime_error);
}