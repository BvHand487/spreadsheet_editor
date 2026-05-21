#ifndef UTILITY_H
#define UTILITY_H

#include <filesystem>
#include <string>
#include <vector>


void split(const std::string &text, char delimiter, std::vector<std::string> &tokens);
std::vector<std::string> split(const std::string &text, char delimiter);

std::string escape(const std::string& text);
std::string unescape(const std::string& text);

int parse_int(const std::string& text);
unsigned int parse_uint(const std::string& text);
float parse_float(const std::string& text);
std::filesystem::path parse_path(const std::string& text);

#endif //UTILITY_H
