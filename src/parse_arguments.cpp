#include <iostream>
#include <string>

#include "parse_arguments.hpp"
#include "decode_command.hpp"
#include "utils.hpp"

void parse_arguments(int argc, char *argv[])
{
	std::string command = argv[1];
	if (command == "decode") {
		decode_command(argc, argv);
	}
	else {
		std::string message = "unknown command: " + command;
		exit_with_message(message); 
	}
}