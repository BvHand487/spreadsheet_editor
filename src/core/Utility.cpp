#include "Utility.h"

#include <sstream>


bool approx_equals(const double a, const double b, const double epsilon)
{
    return std::abs(a - b) <= epsilon;
}

void split(const std::string &text, const char delimiter, std::vector<std::string> &tokens)
{
    std::stringstream ss(text);
    std::string token;

    while (std::getline(ss, token, delimiter))
        if (!token.empty())
            tokens.push_back(token);
}

std::vector<std::string> split(const std::string &text, const char delimiter)
{
    std::vector<std::string> tokens;
    split(text, delimiter, tokens);
    return tokens;
}

std::string getLine(const std::string &text, const size_t line)
{
    size_t lineCount = 0;
    size_t lineStart = 0;

    for (size_t i = 0; i < text.length(); ++i)
    {
        if (text[i] == '\n')
        {
            if (lineCount == line)
                return text.substr(lineStart, i - lineStart);

            lineStart = i + 1;
            lineCount++;
        }
    }

    if (lineCount == line)
        return text.substr(lineStart);

    return "";
}

std::string escape(const std::string &text)
{
    std::string result = "\"";
    result.reserve(2 * text.size());

    for (const char c : text)
    {
        switch (c)
        {
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            default:
                result += c;
        }
    }

    result += "\"";
    return result;
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


std::string trim(const std::string& text)
{
    const std::string whitespace = " \t\r\n";

    const size_t firstIdx = text.find_first_not_of(whitespace);
    if (firstIdx == std::string::npos)
        return "";

    const size_t lastIdx = text.find_last_not_of(whitespace);

    return text.substr(firstIdx, lastIdx - firstIdx + 1);
}


// dd-mm-yyyy
std::tuple<unsigned, unsigned, unsigned> parse_date_numbers(const std::string &text, const char delimiter)
{
    size_t index = 0;
    size_t offset = 0;

    unsigned day = std::stoul(text, &index);
    if (text[offset + index++] != delimiter) throw std::invalid_argument("Unexpected character while parsing date.");

    offset += index;

    unsigned month = std::stoul(&text[offset], &index);
    if (text[offset + index++] != delimiter) throw std::invalid_argument("Unexpected character while parsing date.");

    offset += index;

    unsigned year = std::stoul(&text[offset], &index);

    return std::make_tuple(day, month, year);
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

