#include "doctest.h"
#include <debug_macros.hpp>
#include <lib/containers/bytestream.hpp>

// ========== tests for constructor ===========================================

TEST_CASE("<ByteStream> convert vector successfully into a bytestream")
{
	std::vector<uint8_t> buffer{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	ByteStream bs(buffer);
	CHECK(bs == buffer);
}

// ========== tests for removePrefix ==========================================

TEST_CASE("<ByteStream> remove prefix")
{
	std::vector<uint8_t> buffer{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	ByteStream bs(buffer);

	SUBCASE("zero")
	{
		bs.remove_prefix(0);
		CHECK(bs.size() == buffer.size());
		CHECK(bs.front() == 0);
	}

	SUBCASE("single")
	{
		int test = 1;
		bs.remove_prefix(test);
		CHECK(bs.size() == buffer.size() - test);
		CHECK(bs.front() == test);
	}

	SUBCASE("multi")
	{
		int test = 2;
		bs.remove_prefix(test);
		CHECK(bs.size() == buffer.size() - test);
		CHECK(bs.front() == test);
	}

	SUBCASE("equal to size")
	{
		int test = bs.size();
		bs.remove_prefix(test);
		CHECK(bs.size() == 0);
		CHECK(bs.empty());
	}

	SUBCASE("more than size")
	{
		int test = bs.size() + 1;
		bs.remove_prefix(test);
		CHECK(bs.size() == 0);
		CHECK(bs.empty());
	}
}

// ========== tests for removeSuffix ==========================================

TEST_CASE("<ByteStream> remove suffix")
{
	std::vector<uint8_t> buffer{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
	ByteStream bs(buffer);

	SUBCASE("zero")
	{
		bs.remove_suffix(0);
		CHECK(bs.size() == buffer.size());
		CHECK(bs.back() == 14);
	}

	SUBCASE("single")
	{
		int test = 1;
		bs.remove_suffix(test);
		CHECK(bs.size() == buffer.size() - test);
		CHECK(bs.back() == 13);
	}

	SUBCASE("multi")
	{
		int test = 2;
		bs.remove_suffix(test);
		CHECK(bs.size() == buffer.size() - test);
		CHECK(bs.back() == 12);
	}

	SUBCASE("equal to size")
	{
		int test = bs.size();
		bs.remove_suffix(test);
		CHECK(bs.size() == 0);
		CHECK(bs.empty());
	}

	SUBCASE("more than size")
	{
		int test = bs.size() + 1;
		bs.remove_suffix(test);
		CHECK(bs.size() == 0);
		CHECK(bs.empty());
	}
}

// ========== tests for pop ===================================================

TEST_CASE("<ByteStream> popping a string")
{
	std::vector<uint8_t> buffer{'A', 'B', 'C', 'D', 'E', 'F'};
	ByteStream bs(buffer);
	CHECK(bs.pop_str(3) == "ABC");
	CHECK(bs.size() == 3);
	CHECK(bs.front() == 'D');
}

TEST_CASE("<ByteStream> popping an integer")
{
	std::vector<uint8_t> buffer{0, 0, 20, 100, 1, 2};
	ByteStream bs(buffer);
	CHECK(bs.pop_int() == 5220);
	CHECK(bs.size() == 2);
	CHECK(bs.front() == 1);
}

TEST_CASE("<ByteStream> popping an unsigned integer")
{
	std::vector<uint8_t> buffer{0, 0, 20, 100, 1, 2};
	ByteStream bs(buffer);
	CHECK(bs.pop_uint() == 5220);
	CHECK(bs.size() == 2);
	CHECK(bs.front() == 1);
}

TEST_CASE("<ByteStream> popping a byte")
{
	std::vector<uint8_t> buffer{20, 100, 1, 2};
	ByteStream bs(buffer);
	CHECK(bs.pop_byte() == 20);
	CHECK(bs.size() == 3);
	CHECK(bs.front() == 100);
}

// ========== tests for push ===================================================

TEST_CASE("<ByteStream> pushing a byte")
{
	std::vector<uint8_t> buffer{20, 100, 1, 2};
	ByteStream bs(buffer);
	bs.push_back((uint8_t)10);
	CHECK(bs.back() == 10);
	CHECK(bs.size() == buffer.size() + 1);
}

TEST_CASE("<ByteStream> pushing a int")
{
	std::vector<uint8_t> buffer{20, 100, 1, 2};
	ByteStream bs(buffer);
	bs.push_back(10);
	CHECK(bs[3] == 2);
	CHECK(bs[4] == 0);
	CHECK(bs[5] == 0);
	CHECK(bs[6] == 0);
	CHECK(bs[7] == 10);
	CHECK(bs.size() == buffer.size() + sizeof(int));
}

TEST_CASE("<ByteStream> pushing a unsigned")
{
	std::vector<uint8_t> buffer{20, 100, 1, 2};
	ByteStream bs(buffer);
	bs.push_back((unsigned)10);
	CHECK(bs[3] == 2);
	CHECK(bs[4] == 0);
	CHECK(bs[5] == 0);
	CHECK(bs[6] == 0);
	CHECK(bs[7] == 10);
	CHECK(bs.size() == buffer.size() + sizeof(int));
}

TEST_CASE("<ByteStream> pushing a string")
{
	std::vector<uint8_t> buffer{20, 100, 1, 2};
	ByteStream bs(buffer);
	bs.push_back("ABC");
	CHECK(bs[3] == 2);
	CHECK(bs[4] == 'A');
	CHECK(bs[5] == 'B');
	CHECK(bs[6] == 'C');
	CHECK(bs.size() == buffer.size() + 3);
}
