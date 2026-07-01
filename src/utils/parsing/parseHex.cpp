#include "utils/parsing.hpp"
#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <string>

unsigned long parseHex(const std::string &input) {

	if (!ishexstr(input)) throw std::invalid_argument(input);

	char *endPtr;
	errno = 0;
	long result = strtoul(input.c_str(), &endPtr, 16);

	if (errno == ERANGE) throw std::out_of_range(input);

	return result;
}
