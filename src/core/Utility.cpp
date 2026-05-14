#include "Utility.h"

#include <sstream>

void split(const std::string &text, const char delimiter, std::vector<std::string> &tokens)
{
    std::stringstream ss(text);
    std::string token;

    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
}

std::vector<std::string> split(const std::string &text, const char delimiter)
{
    std::vector<std::string> tokens;
    split(text, delimiter, tokens);
    return tokens;
}
