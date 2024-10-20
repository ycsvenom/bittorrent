#include <iostream>
#include <sstream>
#include <string_view>

#include "decode.hpp"
#include <lib/bencode/bencode.hpp>
#include <lib/utils/utils.hpp>

std::stringstream get_decode_help(const std::string &name)
{
	std::stringstream ss;
	ss << "Usage: " << name << " decode <encoded_value>";
	return ss;
}

void decode_command(int argc, char *argv[])
{
	if (argc < 3) {
		exit_with_message(get_decode_help(argv[0]));
	}

	std::string encoded = argv[2];
	json decoded = decode_bencoded_value(encoded);
	std::cout << decoded.dump() << std::endl;
}