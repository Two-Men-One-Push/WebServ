#include <string>

std::string trim(const std::string &s) {
	const std::string whitespace = " \t";
	size_t start = s.find_first_not_of(whitespace);
	if (start == std::string::npos)
		return "";
	size_t end = s.find_last_not_of(whitespace);
	return s.substr(start, end - start + 1);
}
