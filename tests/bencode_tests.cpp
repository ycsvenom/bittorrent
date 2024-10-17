#include "../src/bencode.hpp"
#include "doctest.h"

// ========== tests for decode_bencoded_value =================================

// test decoding strings

TEST_CASE("empty")
{
	CHECK(decode_bencoded_value("0:").dump() == "\"\"");
}

TEST_CASE("non-empty")
{
	CHECK(decode_bencoded_value("5:hello").dump() == "\"hello\"");
	CHECK(decode_bencoded_value("11:hello world").dump() == "\"hello world\"");
}

TEST_CASE("invalid format")
{
	CHECK_THROWS_AS(decode_bencoded_value("a:hello").dump(), std::runtime_error);
	CHECK_THROWS_AS(decode_bencoded_value("5hello").dump(), std::runtime_error);
}

// test decoding integers

TEST_CASE("zero")
{
	CHECK(decode_bencoded_value("i0e").dump() == "0");
}

TEST_CASE("negative and positive")
{
	CHECK(decode_bencoded_value("i52e").dump() == "52");
	CHECK(decode_bencoded_value("i-52e").dump() == "-52");
}

TEST_CASE("invalid format")
{
	CHECK_THROWS_AS(decode_bencoded_value("i1"), std::runtime_error);
}

// test decoding lists

TEST_CASE("empty")
{
	CHECK(decode_bencoded_value("le").dump() == "[]");
}

TEST_CASE("single element homogenous")
{
	CHECK(decode_bencoded_value("li52ee").dump() == "[52]");
	CHECK(decode_bencoded_value("l5:helloe").dump() == "[\"hello\"]");
}

TEST_CASE("multi element homogenous")
{
	CHECK(decode_bencoded_value("li52ei53ee").dump() == "[52,53]");
	CHECK(decode_bencoded_value("l5:hello5:worlde").dump() == "[\"hello\",\"world\"]");
}

TEST_CASE("multi element heterogenous")
{
	CHECK(decode_bencoded_value("l5:helloi52ee").dump() == "[\"hello\",52]");
	CHECK(decode_bencoded_value("li52e5:helloe").dump() == "[52,\"hello\"]");
}

TEST_CASE("nested elements")
{
	CHECK(decode_bencoded_value("ll5:helloi1eei2ee").dump() == "[[\"hello\",1],2]");
}

TEST_CASE("invaild format")
{
	CHECK_THROWS_AS(decode_bencoded_value("li52e"), std::runtime_error);
	CHECK_THROWS_AS(decode_bencoded_value("l1:h"), std::runtime_error);
}

// test decoding dictionary

TEST_CASE("empty")
{
	CHECK(decode_bencoded_value("de").dump() == "{}");
}

TEST_CASE("single key")
{
	CHECK(decode_bencoded_value("d1:ai1ee").dump() == "{\"a\":1}");
	CHECK(decode_bencoded_value("d1:a3:abce").dump() == "{\"a\":\"abc\"}");
	CHECK(decode_bencoded_value("d1:al3:abcee").dump() == "{\"a\":[\"abc\"]}");
}

TEST_CASE("multi keys")
{
	CHECK(decode_bencoded_value("d3:foo3:bar5:helloi52ee").dump() == "{\"foo\":\"bar\",\"hello\":52}");
	CHECK(decode_bencoded_value("d1:al3:abci12ee1:bi7ee").dump() == "{\"a\":[\"abc\",12],\"b\":7}");
}

TEST_CASE("nested items")
{
	CHECK(decode_bencoded_value("d1:ad3:abci12eee").dump() == "{\"a\":{\"abc\":12}}");
}

TEST_CASE("invalid format")
{
	CHECK_THROWS_AS(decode_bencoded_value("di4ee"), std::runtime_error);
	CHECK_THROWS_AS(decode_bencoded_value("d1:he"), std::runtime_error);
	CHECK_THROWS_AS(decode_bencoded_value("d1:hi2e"), std::runtime_error);
}