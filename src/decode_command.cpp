#include <iostream>
#include <sstream>

#include "decode_command.hpp"
#include "bencode.hpp"
#include "utils.hpp"

std::stringstream getDecodeHelp(const std::string& name)
{
	std::stringstream ss;
	ss << "Usage: " << name << " decode <encoded_value>";
	return ss;
}

void decodeCommand(int argc, char *argv[])
{
	if (argc < 3) {
		exitWithMessage(getDecodeHelp(argv[0]));
	}
	
	// You can use print statements as follows for debugging, they'll be visible when running tests.
	std::string encoded = argv[2];
	std::string decoded = decode(encoded);
	std::cout << decoded << std::endl;
}