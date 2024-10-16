#include "doctest.h"
#include "../src/bencode.hpp"

TEST_CASE("decoding strings decode_bencoded_value")
{
	CHECK(decode_bencoded_value("0:").dump() == "\"\"");
	CHECK(decode_bencoded_value("5:hello").dump() == "\"hello\"");
	CHECK(decode_bencoded_value("11:hello world").dump() == "\"hello world\"");
	CHECK_THROWS_AS(decode_bencoded_value("a:hello").dump(), std::runtime_error);
	CHECK_THROWS_AS(decode_bencoded_value("5hello").dump(), std::runtime_error);
}

TEST_CASE("decoding integers decode_bencoded_value")
{
	CHECK(decode_bencoded_value("i52e") == "52");
	CHECK(decode_bencoded_value("i0e") == "0");
	CHECK(decode_bencoded_value("i-52e") == "-52");
}