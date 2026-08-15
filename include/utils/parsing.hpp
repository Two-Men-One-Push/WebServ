#ifndef PARSINGHPP
#define PARSINGHPP

#include <ctime>
#include <string>
#include <vector>

bool ishexdigit(char c);
bool ishexstr(const std::string &s);
bool isseparator(char c);
bool istokenc(unsigned char c);
bool istoken(const std::string &s);
bool isheadervalue(const std::string &s);

void split(const std::string &s, const std::string &delim, std::vector<std::string> &out);
std::string trim(const std::string &s, const std::string &set = " \t");
std::string &toHeaderCase(std::string &s);

long parseLong(const std::string &input);
unsigned long parseULong(const std::string &input);
unsigned long parseHex(const std::string &input);

bool	parseInt(const std::string &str, int &out);
bool	parseTime(const std::string &str, std::time_t &out);
bool	parseSize(const std::string &str, size_t &out);

std::string	trim_path(const std::string &path);
std::string	pathJoin(const std::string &path1, const std::string &path2);
std::vector<std::string>	splitPath(const std::string &str);

/*
return true if the path unwinds to a path outside the root, false otherwise
*/
bool	pathNormalize(std::string &result, const std::string &path);
std::string	mergePathSegments(const std::vector<std::string> &segments);

#endif
