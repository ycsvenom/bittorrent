#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "parse_arguments.hpp"
#include "utils.hpp"

int main(int argc, char *argv[])
{
	// Flush after every std::cout / std::cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	if (argc < 2) {
		exitWithMessage(argv[0]);
	}

	parse(argc, argv);

	return 0;
}
