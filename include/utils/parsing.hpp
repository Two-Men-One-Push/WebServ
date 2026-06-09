#ifndef PARSINGHPP
#define PARSINGHPP

#include <string>
#include <vector>


bool ishighhexchar(char c);
bool islowhexchar(char c);
bool ishexdigit(char c);
bool isseparator(char c);
bool istokenc(char c);
bool istoken(const std::string &s);

void split(const std::string &s, const std::string &delim, std::vector<std::string> &out);
std::string trim(const std::string &s);
std::string &toHeaderCase(std::string &s);

long parseLong(const std::string &input);
unsigned long parseULong(const std::string &input);

#endif
