#include "bencode.hpp"

std::string decode(std::string &encodedValue)
{
	json decoded_value = decode_bencoded_value(encodedValue);
	return decoded_value.dump();
}

std::string decode_string(const std::string &encoded_value, int start)
{
	// Example: "5:hello" -> "hello"
	size_t colon_index = encoded_value.find(':', start);

	if (colon_index == std::string::npos)
		throw std::runtime_error("Invalid encoded value: " + encoded_value);

	std::string size_string = encoded_value.substr(start, colon_index);
	int64_t size = std::atoll(size_string.c_str());
	std::string str = encoded_value.substr(colon_index + 1, size);

	return str;
}

int64_t decode_integer(const std::string &encoded_value, int start)
{
	// Example: "i52e" -> 52
	size_t end_index = encoded_value.find('e', start);

	if (end_index == std::string::npos)
		throw std::runtime_error("Invalid encoded value: " + encoded_value);

	std::string number_string = encoded_value.substr(start + 1, end_index);
	int64_t number = std::atoll(number_string.c_str());

	return number;
}

json decode_bencoded_value(const std::string &encoded_value)
{
	char identifier = encoded_value[0];

	if (std::isdigit(identifier))
		return json(decode_string(encoded_value, 0));
	else if (identifier == 'i')
		return json(decode_integer(encoded_value, 0));
	else
		throw std::runtime_error("Unhandled encoded value: " + encoded_value);
}
