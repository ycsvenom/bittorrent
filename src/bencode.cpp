#include "bencode.hpp"

std::string decode(std::string &encodedValue)
{
	json decoded_value = decode_bencoded_value(encodedValue);
	return decoded_value.dump();
}

json decode_bencoded_value(const std::string &encoded_value)
{
	if (!std::isdigit(encoded_value[0]))
		throw std::runtime_error("Unhandled encoded value: " + encoded_value);

	// Example: "5:hello" -> "hello"
	size_t colon_index = encoded_value.find(':');

	if (colon_index == std::string::npos)
		throw std::runtime_error("Invalid encoded value: " + encoded_value);

	std::string size_string = encoded_value.substr(0, colon_index);
	int64_t size = std::atoll(size_string.c_str());
	std::string str = encoded_value.substr(colon_index + 1, size);
	return json(str);
}