#ifndef BENCODE_H
#define BENCODE_H

#include <string>

#include "lib/nlohmann/json.hpp"

using json = nlohmann::json;

std::string decode(std::string &encodedValue);
json decode_bencoded_value(const std::string &encoded_value);

#endif