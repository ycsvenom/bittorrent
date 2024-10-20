#include <string_view>

#include "bencode.hpp"
#include <debug_macros.hpp>

#define IS_STR(id) std::isdigit(id)
#define IS_INT(id) id == INT_IDEN
#define IS_LIST(id) id == LIST_IDEN
#define IS_DICT(id) id == DICT_IDEN

json decode(std::string_view &);

// Example: "5:hello" -> "hello"
json decode_string(std::string_view &encodedValue)
{
	size_t colonIndex = encodedValue.find(STR_SEP);

	if (colonIndex == std::string_view::npos)
		throw std::invalid_argument("Invalid encoded value: " + std::string(encodedValue));

	std::string_view sizeString = encodedValue.substr(0, colonIndex);
	int64_t size = std::atoll(sizeString.data());
	std::string_view str = encodedValue.substr(colonIndex + 1, size);
	encodedValue.remove_prefix(colonIndex + size + sizeof(STR_SEP));

	return json(str);
}

// Example: "i52e" -> 52
json decode_integer(std::string_view &encodedValue)
{
	encodedValue.remove_prefix(sizeof(INT_IDEN));
	size_t endIndex = encodedValue.find(EOS);

	if (endIndex == std::string::npos)
		throw std::invalid_argument("Invalid encoded value: " + std::string(encodedValue));

	std::string_view numberString = encodedValue.substr(0, endIndex);
	int64_t number = std::atoll(numberString.data());
	encodedValue.remove_prefix(endIndex + sizeof(EOS));

	return json(number);
}

// Example: "l5:helloi52ee" -> ["hello", 52]
json decode_list(std::string_view &encodedValue)
{
	encodedValue.remove_prefix(sizeof(LIST_IDEN));
	std::vector<json> list;

	while (!encodedValue.empty() && encodedValue.front() != EOS) {
		list.push_back(decode(encodedValue));
	}

	if (encodedValue.empty() || encodedValue.front() != EOS)
		throw std::invalid_argument("Invalid encoded value: " + std::string(encodedValue));

	encodedValue.remove_prefix(sizeof(EOS));
	return json(list);
}

// Example: "d3:foo3:bar5:helloi52ee" -> {"hello": 52, "foo":"bar"}
json decode_dict(std::string_view &encodedValue)
{
	encodedValue.remove_prefix(sizeof(DICT_IDEN));
	std::map<json, json> dict;

	while (!encodedValue.empty() && encodedValue.front() != EOS) {
		json key = decode(encodedValue);
		json value = decode(encodedValue);
		dict[key] = value;
	}

	if (encodedValue.empty() || encodedValue.front() != EOS)
		throw std::invalid_argument("Invalid encoded value: " + std::string(encodedValue));

	encodedValue.remove_prefix(sizeof(EOS));
	return json(dict);
}

json decode(std::string_view &encodedValue)
{
	json decoded;
	char identifier = encodedValue.front();

	if (IS_STR(identifier))
		decoded = decode_string(encodedValue);
	else if (IS_INT(identifier))
		decoded = decode_integer(encodedValue);
	else if (IS_LIST(identifier))
		decoded = decode_list(encodedValue);
	else if (IS_DICT(identifier))
		decoded = decode_dict(encodedValue);
	else
		throw std::invalid_argument("Unhandled encoded value: " + std::string(encodedValue));

	return decoded;
}

json decode_bencoded_value(const std::string &encodedValue)
{
	std::string_view sv(encodedValue);
	json decoded = decode(sv);
	return decoded;
}