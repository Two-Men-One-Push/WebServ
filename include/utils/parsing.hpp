#ifndef PARSINGHPP
#define PARSINGHPP

#include <string>
#include <vector>

bool isseparator(char c);
bool istokenc(char c);
bool istoken(const std::string &s);

void split(const std::string &s, const std::string &delim, std::vector<std::string>& out);

#endif
