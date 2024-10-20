#include <iomanip>
#include <iostream>
#include <random>
#include <set>
#include <time.h>

#include "utils.hpp"

#include <debug_macros.hpp>

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

void clear_stringstream(std::stringstream &ss, const std::string &value)
{
	ss.clear();
	ss.str(value);
}

std::string bin_to_hex(const std::string &bin)
{
	std::stringstream ss;
	for (size_t i = 0; i < bin.length(); i++) {
		unsigned char c = bin[i];
		ss << std::setfill('0') << std::setw(2) << std::hex << (int)c;
	}
	return ss.str();
}

std::string hex_to_bin(const std::string &hex)
{
	std::stringstream ss;
	for (size_t i = 0; i < hex.length(); i += 2) {
		int val;
		std::stringstream reader;
		reader << std::hex << hex.substr(i, 2);
		reader >> val;
		ss << (uint8_t)val;
	}
	return ss.str();
}

std::string url_encode(const std::string &text)
{
	std::stringstream ss;
	std::string unreserved = URL_UNRESERVED_CHAR;
	std::set pile(unreserved.begin(), unreserved.end());

	for (const char &c : text) {
		if (pile.count(c)) {
			ss << c;
		}
		else {
			unsigned char t = c;
			ss << '%'
			   << std::setfill('0')
			   << std::setw(2)
			   << std::uppercase
			   << std::hex
			   << (int)t;
		}
	}

	return ss.str();
}

std::string gen_hash(size_t length)
{
	std::stringstream ss;
	std::string pile = "abcdef0123456789";

	for (size_t i = 0; i < length; i++) {
		ss << pile[randint(pile.length())];
	}

	return ss.str();
}

int randint(int min, int max)
{
	srand(time(NULL));
	return (rand() + min) % max;
}

int randint(int max)
{
	return randint(0, max);
}