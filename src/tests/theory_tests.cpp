#include <sstream>
#include <string>

#include "doctest.h"

TEST_CASE("test string related theories")
{
	std::stringstream ss;
	ss << "hello" << (u_int8_t)0 << (u_int8_t)0 << " world";

	SUBCASE("test length of null bytes in stringstream")
	{
		CHECK(ss.str().length() == 13);
	}
}