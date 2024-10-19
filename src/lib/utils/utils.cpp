#include <iomanip>
#include <iostream>

#include "utils.hpp"

void print(const std::string &text, bool isError)
{
	if (isError)
		std::cerr << text << std::endl;
	else
		std::cout << text << std::endl;
}

void print(const std::stringstream &ss, bool isError)
{
	print(ss.str(), isError);
}

void exit_with_message(const std::string &text)
{
	print(text, true);
	std::exit(1);
}

void exit_with_message(const std::stringstream &ss)
{
	exit_with_message(ss.str());
}

std::string binary_string_to_hex(const std::string binaryString)
{
	std::stringstream ss;
	for (size_t i = 0; i < binaryString.length(); i++) {
		unsigned char c = binaryString[i];
		ss << std::setfill('0') << std::setw(2) << std::hex << (int)c;
	}
	return ss.str();
}