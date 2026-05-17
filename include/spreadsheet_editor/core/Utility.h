#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <vector>

void split(const std::string &text, char delimiter, std::vector<std::string> &tokens);
std::vector<std::string> split(const std::string &text, char delimiter);

#endif //UTILITY_H
