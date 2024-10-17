#include <iostream>

#include "parse_arguments.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
	// Flush after every std::cout / std::cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	if (argc < 2) {
		exit_with_message(argv[0]);
	}

	parse_arguments(argc, argv);

	return 0;
}
