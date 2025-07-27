#include "doctest.h"
#include <lib/hash/sha1.h>

// ========== tests for sha1_hash =============================================

TEST_CASE("<SHA1> test for empty string")
{
	SHA1 sha1;
	CHECK(sha1("") == "da39a3ee5e6b4b0d3255bfef95601890afd80709");
	CHECK(sha1("hello") == "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d");
}