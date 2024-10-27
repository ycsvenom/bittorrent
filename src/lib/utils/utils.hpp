#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>

#define URL_UNRESERVED_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~"

void print(const std::string &text, bool isError);
void print(const std::stringstream &ss, bool isError);
void exit_with_message(const std::string &text);
void exit_with_message(const std::stringstream &ss);
void clear_stringstream(std::stringstream &ss, const std::string &value = std::string());
std::string bin_to_hex(const std::string &bin);
std::string hex_to_bin(const std::string &hex);
std::string url_encode(const std::string &text);
std::string gen_hash(size_t length);
int randint(int min, int max);
int randint(int max);
int bstoi(const std::string &binString);
std::string bitos(int x);

#endif