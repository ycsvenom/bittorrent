#include <iostream>
#include <string>

#include "parse_arguments.hpp"
#include <commands/decode.hpp>
#include <commands/display_info.hpp>
#include <lib/utils/utils.hpp>

void parse_arguments(int argc, char *argv[])
{
	std::string command = argv[1];
	if (command == "decode") {
		decode_command(argc, argv);
	}
	else if (command == "info") {
		display_info_command(argc, argv);
	}
	else {
		std::string message = "unknown command: " + command;
		exit_with_message(message);
	}
}