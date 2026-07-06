#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <string>

long parseLong(const std::string &input) {
	errno = 0;

	char *endPtr;
	long result = strtol(input.c_str(), &endPtr, 10);

	if (errno == ERANGE) throw std::out_of_range(input);

	return result;
}

unsigned long parseULong(const std::string &input) {
	errno = 0;

	char *endPtr;
	long result = strtoul(input.c_str(), &endPtr, 10);

	if (errno == ERANGE) throw std::out_of_range(input);

	return result;
}
