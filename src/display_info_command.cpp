#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "bencode.hpp"
#include "display_info_command.hpp"
#include "lib/nlohmann/json.hpp"
#include "utils.hpp"

std::stringstream get_display_info_help(const std::string &name)
{
	std::stringstream ss;
	ss << "Usage: " << name << " info <torrent_path>";
	return ss;
}

void display_info_command(int argc, char *argv[])
{
	if (argc < 3) {
		exit_with_message(get_display_info_help(argv[0]));
	}

	std::string path = argv[2];
	std::ifstream file(path, std::ios_base::in);
	std::stringstream ss;
	ss << file.rdbuf();

	json decoded = decode_bencoded_value(ss.str());
	json announce = decoded["announce"];
	json info = decoded["info"];
	json infoLength = info["length"];

	std::cout << "Tracker URL: " << announce.get<std::string>() << std::endl;
	std::cout << "Length: " << infoLength.dump() << std::endl;
}