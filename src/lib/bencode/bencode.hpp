#pragma once
#ifndef BENCODE_H
#define BENCODE_H

#include <string>

#include <lib/nlohmann/json.hpp>

// end of segment
#define EOS 'e'
// string seperator
#define STR_SEP ':'

#define INT_IDEN 'i'
#define LIST_IDEN 'l'
#define DICT_IDEN 'd'

using json = nlohmann::json;

json decode_bencoded_value(const std::string &encodedValue);
std::string encode_json_to_bencode(const json &value);

#endif