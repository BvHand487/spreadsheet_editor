#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>


bool approx_equals(double a, double b, double epsilon = 1e-12);

void split(const std::string &text, char delimiter, std::vector<std::string> &tokens);
std::vector<std::string> split(const std::string &text, char delimiter);

std::string getLine(const std::string &text, size_t line = 0);

std::string escape(const std::string& text);
std::string unescape(const std::string& text);

// removes whitespaces around a string
std::string trim(const std::string& text);

std::tuple<unsigned, unsigned, unsigned> parse_date_numbers(const std::string& text, char delimiter = '/');
int parse_int(const std::string& text);
unsigned int parse_uint(const std::string& text);
float parse_float(const std::string& text);

#endif //UTILITY_H
