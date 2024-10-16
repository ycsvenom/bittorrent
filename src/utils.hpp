#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>

void print(const std::string &text, bool isError);
void print(const std::stringstream &ss, bool isError);
void exitWithMessage(const std::string &text);
void exitWithMessage(const std::stringstream &ss);

#endif