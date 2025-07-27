#include "doctest.h"
#include <lib/bencode/bencode.hpp>
#include <lib/nlohmann/json.hpp>

// ========== tests for decode_bencoded_value =================================

// test decoding strings

TEST_CASE("<bencode> empty")
{
	CHECK(decode_bencoded_value("0:").dump() == "\"\"");
}

TEST_CASE("<bencode> non-empty")
{
	CHECK(decode_bencoded_value("5:hello").dump() == "\"hello\"");
	CHECK(decode_bencoded_value("11:hello world").dump() == "\"hello world\"");
}

TEST_CASE("<bencode> invalid format")
{
	REQUIRE_THROWS_AS(decode_bencoded_value("a:hello").dump(), std::invalid_argument);
	REQUIRE_THROWS_AS(decode_bencoded_value("5hello").dump(), std::invalid_argument);
}

// test decoding integers

TEST_CASE("<bencode> zero")
{
	CHECK(decode_bencoded_value("i0e").dump() == "0");
}

TEST_CASE("<bencode> negative and positive")
{
	CHECK(decode_bencoded_value("i52e").dump() == "52");
	CHECK(decode_bencoded_value("i-52e").dump() == "-52");
}

TEST_CASE("<bencode> invalid format")
{
	REQUIRE_THROWS_AS(decode_bencoded_value("i1"), std::invalid_argument);
}

// test decoding lists

TEST_CASE("<bencode> empty")
{
	CHECK(decode_bencoded_value("le").dump() == "[]");
}

TEST_CASE("<bencode> single element")
{
	CHECK(decode_bencoded_value("li52ee").dump() == "[52]");
	CHECK(decode_bencoded_value("l5:helloe").dump() == "[\"hello\"]");
}

TEST_CASE("<bencode> multi element homogeneous")
{
	CHECK(decode_bencoded_value("li52ei53ee").dump() == "[52,53]");
	CHECK(decode_bencoded_value("l5:hello5:worlde").dump() == "[\"hello\",\"world\"]");
}

TEST_CASE("<bencode> multi element heterogeneous")
{
	CHECK(decode_bencoded_value("l5:helloi52ee").dump() == "[\"hello\",52]");
	CHECK(decode_bencoded_value("li52e5:helloe").dump() == "[52,\"hello\"]");
}

TEST_CASE("<bencode> nested elements")
{
	CHECK(decode_bencoded_value("ll5:helloi1eei2ee").dump() == "[[\"hello\",1],2]");
}

TEST_CASE("<bencode> invaild format")
{
	REQUIRE_THROWS_AS(decode_bencoded_value("li52e"), std::invalid_argument);
	REQUIRE_THROWS_AS(decode_bencoded_value("l1:h"), std::invalid_argument);
}

// test decoding dictionary

TEST_CASE("<bencode> empty")
{
	CHECK(decode_bencoded_value("de").dump() == "{}");
}

TEST_CASE("<bencode> single key")
{
	CHECK(decode_bencoded_value("d1:ai1ee").dump() == "{\"a\":1}");
	CHECK(decode_bencoded_value("d1:a3:abce").dump() == "{\"a\":\"abc\"}");
	CHECK(decode_bencoded_value("d1:al3:abcee").dump() == "{\"a\":[\"abc\"]}");
}

TEST_CASE("<bencode> multi keys")
{
	CHECK(decode_bencoded_value("d3:foo3:bar5:helloi52ee").dump() == "{\"foo\":\"bar\",\"hello\":52}");
	CHECK(decode_bencoded_value("d1:al3:abci12ee1:bi7ee").dump() == "{\"a\":[\"abc\",12],\"b\":7}");
}

TEST_CASE("<bencode> nested items")
{
	CHECK(decode_bencoded_value("d1:ad3:abci12eee").dump() == "{\"a\":{\"abc\":12}}");
}

TEST_CASE("<bencode> invalid format")
{
	REQUIRE_THROWS_AS(decode_bencoded_value("di4ee"), std::invalid_argument);
	REQUIRE_THROWS_AS(decode_bencoded_value("d1:he"), std::invalid_argument);
	REQUIRE_THROWS_AS(decode_bencoded_value("d1:hi2e"), std::invalid_argument);
}

// ========== tests for encode_json_to_bencode =================================

// test encoding strings

TEST_CASE("<bencode> empty")
{
	json elem = "";
	CHECK(encode_json_to_bencode(elem) == "0:");
}

TEST_CASE("<bencode> non-empty")
{
	json elem = "hello";
	CHECK(encode_json_to_bencode(elem) == "5:hello");
}

// test encoding integer

TEST_CASE("<bencode> zero")
{
	json elem = 0;
	CHECK(encode_json_to_bencode(elem) == "i0e");
}

TEST_CASE("<bencode> positive")
{
	json elem = 5;
	CHECK(encode_json_to_bencode(elem) == "i5e");
}

TEST_CASE("<bencode> negative")
{
	json elem = -5;
	CHECK(encode_json_to_bencode(elem) == "i-5e");
}

// test encoding list

TEST_CASE("<bencode> empty")
{
	json list = json::array();
	CHECK(encode_json_to_bencode(list) == "le");
}

TEST_CASE("<bencode> single element")
{
	json listInt = {1};
	json listStr = {"hello"};
	CHECK(encode_json_to_bencode(listInt) == "li1ee");
	CHECK(encode_json_to_bencode(listStr) == "l5:helloe");
}

TEST_CASE("<bencode> multi elements homogenous")
{
	json listInt = {1, 2, 3};
	json listStr = {"hello", "world"};
	CHECK(encode_json_to_bencode(listInt) == "li1ei2ei3ee");
	CHECK(encode_json_to_bencode(listStr) == "l5:hello5:worlde");
}

TEST_CASE("<bencode> multi elements heterogeneous")
{
	json list = {1, "hello", 2, "world"};
	CHECK(encode_json_to_bencode(list) == "li1e5:helloi2e5:worlde");
}

TEST_CASE("<bencode> nested")
{
	json list = {1, {"hello", 2}, "world"};
	CHECK(encode_json_to_bencode(list) == "li1el5:helloi2ee5:worlde");
}

// test encoding dictionary

TEST_CASE("<bencode> empty")
{
	json dict = json::object();
	CHECK(encode_json_to_bencode(dict) == "de");
}

TEST_CASE("<bencode> single item")
{
	json dict = {{"one", 1}};
	CHECK(encode_json_to_bencode(dict) == "d3:onei1ee");
}

TEST_CASE("<bencode> multi items")
{
	json dict = {
		{"one", 1},
		{"two", 2},
	};
	CHECK(encode_json_to_bencode(dict) == "d3:onei1e3:twoi2ee");
}

TEST_CASE("<bencode> nested")
{
	json dict = {
		{"one", {{"two", 2}}},
	};
	CHECK(encode_json_to_bencode(dict) == "d3:oned3:twoi2eee");
}