#include "bencode.hpp"
#include "debug_macros.hpp"

json decode(const std::string &, size_t &);

json decode_string(const std::string &encodedValue, size_t &index)
{
	// Example: "5:hello" -> "hello"
	size_t colonIndex = encodedValue.find(':', index);
	size_t sizeStringLength = colonIndex - index;

	if (colonIndex == std::string::npos)
		throw std::runtime_error("Invalid encoded value: " + encodedValue);

	std::string sizeString = encodedValue.substr(index, sizeStringLength);
	int64_t size = std::atoll(sizeString.c_str());
	std::string str = encodedValue.substr(colonIndex + 1, size);
	index += sizeStringLength + size;

	return json(str);
}

json decode_integer(const std::string &encodedValue, size_t &index)
{
	// Example: "i52e" -> 52
	size_t endIndex = encodedValue.find('e', index);
	size_t numberStringLength = endIndex - index - 1;

	if (endIndex == std::string::npos)
		throw std::runtime_error("Invalid encoded value: " + encodedValue);

	std::string numberString = encodedValue.substr(index + 1, numberStringLength);
	int64_t number = std::atoll(numberString.c_str());
	index += numberStringLength + 1;

	return json(number);
}

json decode_list(const std::string &encodedValue, size_t &index)
{
	// Example: "l5:helloi52ee" -> ["hello", 52]
	std::vector<json> elements;

	while (++index < encodedValue.length() && encodedValue[index] != 'e') {
		elements.push_back(decode(encodedValue, index));
	}

	if (index == encodedValue.length() || encodedValue[index] != 'e')
		throw std::runtime_error("Invalid encoded value: " + encodedValue);

	return json(elements);
}

json decode(const std::string &encodedValue, size_t &index)
{
	json decoded;
	char identifier = encodedValue[index];

	if (std::isdigit(identifier))
		decoded = decode_string(encodedValue, index);
	else if (identifier == 'i')
		decoded = decode_integer(encodedValue, index);
	else if (identifier == 'l')
		decoded = decode_list(encodedValue, index);
	else
		throw std::runtime_error("Unhandled encoded value: " + encodedValue);

	return decoded;
}

json decode_bencoded_value(const std::string &encodedValue)
{
	size_t index = 0;
	json decoded = decode(encodedValue, index);
	return decoded;
}