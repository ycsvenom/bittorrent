#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>

void print(const std::string &text, bool isError);
void print(const std::stringstream &ss, bool isError);
void exit_with_message(const std::string &text);
void exit_with_message(const std::stringstream &ss);

#endif