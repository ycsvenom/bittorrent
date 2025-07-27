#include <stdint.h>

#include "doctest.h"
#include <lib/utils/utils.hpp>

// ========== tests for bin_to_hex ============================================

TEST_CASE("<Utils> empty")
{
	CHECK(bin_to_hex("") == "");
}

TEST_CASE("<Utils> single")
{
	CHECK(bin_to_hex("\x01") == "01");
}

TEST_CASE("<Utils> multi string")
{
	CHECK(bin_to_hex("hello") == "68656c6c6f");
}

TEST_CASE("<Utils> multi binary")
{
	CHECK(bin_to_hex("\x01\x02\x03\x04\x05") == "0102030405");
}

// ========== tests for hex_to_bin ============================================

TEST_CASE("<Utils> empty")
{
	CHECK(hex_to_bin("") == "");
}

TEST_CASE("<Utils> single")
{
	CHECK(hex_to_bin("01") == "\x01");
}

TEST_CASE("<Utils> multi string")
{
	CHECK(hex_to_bin("68656c6c6f") == "hello");
}

TEST_CASE("<Utils> multi binary")
{
	CHECK(hex_to_bin("0102030405") == "\x01\x02\x03\x04\x05");
}

// ========== tests for gen_hash ==============================================

TEST_CASE("<Utils> test length")
{
	size_t size = 20;
	CHECK(gen_hash(size).length() == size);
}

// ========== tests for url_encode ==============================================

TEST_CASE("<Utils> empty")
{
	CHECK(url_encode("") == "");
}

TEST_CASE("<Utils> unreserved characters remain unchanged")
{
	std::string unreserved = URL_UNRESERVED_CHAR;
	CHECK(url_encode(unreserved) == unreserved);
}

TEST_CASE("<Utils> single reserved character changed to percent-encoding")
{
	CHECK(url_encode(" ") == "%20");
	CHECK(url_encode("*") == "%2A");
}

TEST_CASE("<Utils> multi reserved characters changed to percent-encoding")
{
	CHECK(url_encode("!#$&'()*+,/:;=?@[]") == "%21%23%24%26%27%28%29%2A%2B%2C%2F%3A%3B%3D%3F%40%5B%5D");
}

TEST_CASE("<Utils> binary to percent-encoding")
{
	CHECK(url_encode("\x01\x02\x03") == "%01%02%03");
}

TEST_CASE("<Utils> mix")
{
	CHECK(url_encode("hello world\x10") == "hello%20world%10");
}

// ========== tests for clear_stringstream ====================================

TEST_CASE("<Utils> clear")
{
	std::stringstream ss;
	ss << "hello";
	clear_stringstream(ss);
	CHECK(ss.str() == "");
}

// ========== tests for bstoi ====================================

TEST_CASE("<Utils> zeros")
{
	std::stringstream ss;
	ss << (uint8_t)0
	   << (uint8_t)0
	   << (uint8_t)0
	   << (uint8_t)0;
	CHECK(bstoi(ss.str()) == 0);
}

TEST_CASE("<Utils> non-zero")
{
	std::stringstream ss;
	ss << (uint8_t)0
	   << (uint8_t)0
	   << (uint8_t)20
	   << (uint8_t)100;
	CHECK(bstoi(ss.str()) == 5220);
}

// ========== tests for bitos ====================================

TEST_CASE("<Utils> zeros")
{
	std::stringstream ss;
	ss << (uint8_t)0
	   << (uint8_t)0
	   << (uint8_t)0
	   << (uint8_t)0;
	CHECK(ss.str().size() == sizeof(int));
}

TEST_CASE("<Utils> zeros")
{
	std::stringstream ss;
	ss << (uint8_t)0
	   << (uint8_t)0
	   << (uint8_t)0
	   << (uint8_t)0;
	CHECK(bitos(0) == ss.str());
}

TEST_CASE("<Utils> non-zero")
{
	std::stringstream ss;
	ss << (uint8_t)0
	   << (uint8_t)0
	   << (uint8_t)20
	   << (uint8_t)100;
	CHECK(bitos(5220) == ss.str());
}