#ifndef PARSINGHPP
#define PARSINGHPP

#include <string>
#include <vector>

bool isseparator(char c);
bool istokenc(char c);
bool istoken(const std::string &s);

void split(const std::string &s, const std::string &delim, std::vector<std::string> &out);
std::string trim(const std::string &s);
std::string &toHeaderCase(std::string &s);

long parseLong(const std::string &input);
unsigned long parseULong(const std::string &input);

bool	parseInt(const std::string &str, int &out);
bool	parseSize(const std::string &str, size_t &out);

#endif
