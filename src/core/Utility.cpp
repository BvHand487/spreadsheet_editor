#include "Utility.h"

#include <filesystem>
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

std::string escape(const std::string &text)
{
    return "";
}

std::string unescape(const std::string &text)
{
    std::string result;
    result.reserve(text.size());

    for (auto iter = text.begin(); iter != text.end(); ++iter)
    {
        if (*iter == '"') continue;
        if (*iter == '\\')
        {
            if (iter + 1 >= text.end()) {
                result += '\\';
                break;
            }

            ++iter;
            switch (*iter)
            {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                default:
                    result += '\\';
                    result += *iter;
                break;
            }

            continue;
        }

        result += *iter;
    }

    return result;
}

int parse_int(const std::string &text)
{
    const int result = std::stoi(text);
    return result;
}

unsigned int parse_uint(const std::string &text)
{
    const unsigned int result = std::stoul(text);
    return result;
}

float parse_float(const std::string &text)
{
    return std::stof(text);
}

