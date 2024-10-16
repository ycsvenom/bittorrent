#include <iostream>
#include <string>

#include "parse_arguments.hpp"
#include "decode_command.hpp"
#include "utils.hpp"

void parse(int argc, char *argv[])
{
	std::string command = argv[1];
	if (command == "decode") {
		decodeCommand(argc, argv);
	}
	else {
		std::string message = "unknown command: " + command;
		exitWithMessage(message); 
	}
}