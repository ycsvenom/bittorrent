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