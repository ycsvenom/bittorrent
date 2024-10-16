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

void exitWithMessage(const std::string &text)
{
	print(text, true);
	std::exit(1);
}

void exitWithMessage(const std::stringstream &ss)
{
	exitWithMessage(ss.str());
}