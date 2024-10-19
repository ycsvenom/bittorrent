#include "bencode.hpp"
#include <debug_macros.hpp>

#define IS_STR(id) std::isdigit(id)
#define IS_INT(id) id == INT_IDEN
#define IS_LIST(id) id == LIST_IDEN
#define IS_DICT(id) id == DICT_IDEN

json decode(const std::string &, size_t &);

json decode_string(const std::string &encodedValue, size_t &index)
{
	// Example: "5:hello" -> "hello"
	size_t colonIndex = encodedValue.find(STR_SEP, index);
	size_t sizeStringLength = colonIndex - index;

	if (colonIndex == std::string::npos)
		throw std::runtime_error("Invalid encoded value: " + encodedValue);

	std::string sizeString = encodedValue.substr(index, sizeStringLength);
	int64_t size = std::atoll(sizeString.c_str());
	std::string str = encodedValue.substr(colonIndex + 1, size);
	index += sizeStringLength + size + 1;

	return json(str);
}

json decode_integer(const std::string &encodedValue, size_t &index)
{
	// Example: "i52e" -> 52
	size_t endIndex = encodedValue.find(EOS, index);
	size_t numberStringLength = endIndex - index - 1;

	if (endIndex == std::string::npos)
		throw std::runtime_error("Invalid encoded value: " + encodedValue);

	std::string numberString = encodedValue.substr(index + 1, numberStringLength);
	int64_t number = std::atoll(numberString.c_str());
	index += numberStringLength + 2;

	return json(number);
}

json decode_list(const std::string &encodedValue, size_t &index)
{
	// Example: "l5:helloi52ee" -> ["hello", 52]
	std::vector<json> list;

	while (index < encodedValue.length() && encodedValue[index] != EOS) {
		list.push_back(decode(encodedValue, index));
	}

	if (index >= encodedValue.length() || encodedValue[index] != EOS)
		throw std::runtime_error("Invalid encoded value: " + encodedValue);

	// move to next segment
	index++;
	return json(list);
}

json decode_dict(const std::string &encodedValue, size_t &index)
{
	// Example: "d3:foo3:bar5:helloi52ee" -> {"hello": 52, "foo":"bar"}
	std::map<json, json> dict;

	while (index < encodedValue.length() && encodedValue[index] != EOS) {
		json key = decode(encodedValue, index);
		json value = decode(encodedValue, index);
		dict[key] = value;
	}

	if (index >= encodedValue.length() || encodedValue[index] != EOS)
		throw std::runtime_error("Invalid encoded value: " + encodedValue);

	return json(dict);
}

json decode(const std::string &encodedValue, size_t &index)
{
	json decoded;
	char identifier = encodedValue[index];

	if (IS_STR(identifier))
		decoded = decode_string(encodedValue, index);
	else if (IS_INT(identifier))
		decoded = decode_integer(encodedValue, index);
	else if (IS_LIST(identifier))
		decoded = decode_list(encodedValue, ++index);
	else if (IS_DICT(identifier))
		decoded = decode_dict(encodedValue, ++index);
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