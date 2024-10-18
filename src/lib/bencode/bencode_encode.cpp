#include <map>
#include <vector>

#include "bencode.hpp"
#include "../../debug_macros.hpp"

std::string encode_string_to_bencode(const json &str)
{
	std::stringstream ss;
	std::string text = str.get<std::string>();
	size_t size = text.length();
	ss << size << STR_SEP << text;
	return ss.str();
}

std::string encode_integer_to_bencode(const json &str)
{
	std::stringstream ss;
	std::string number = str.dump();
	ss << INT_IDEN << number << EOS;
	return ss.str();
}

std::string encode_list_to_bencode(const json &str)
{
	std::stringstream ss;
	std::vector<json> list = str.get<std::vector<json>>();

	ss << LIST_IDEN;
	for (json elem : list) {
		ss << encode_json_to_bencode(elem);
	}
	ss << EOS;

	return ss.str();
}

std::string encode_dict_to_bencode(const json &str)
{
	std::stringstream ss;
	std::map<json, json> dict = str.get<std::map<json, json>>();

	ss << DICT_IDEN;
	for (auto item : dict) {
		ss << encode_string_to_bencode(item.first) << encode_json_to_bencode(item.second);
	}
	ss << EOS;

	return ss.str();
}

std::string encode_json_to_bencode(const json &value)
{
	std::string encoded;
    
	if (value.is_string())
		encoded = encode_string_to_bencode(value);
	else if (value.is_number_integer())
		encoded = encode_integer_to_bencode(value);
	else if (value.is_array())
		encoded = encode_list_to_bencode(value);
	else if (value.is_object())
		encoded = encode_dict_to_bencode(value);
	else
		throw std::runtime_error("Unhandled json type: " + value.dump());

	return encoded;
}