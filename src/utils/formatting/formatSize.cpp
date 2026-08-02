#include <cstddef>
#include <sstream>
#include <string>

std::string formatSize(std::size_t n) {
	std::stringstream ss;

	if (n < 1000) ss << n << 'o';
	else if (n < 1000000) ss << n / 1000 << "Ko";
	else if (n < 1000000000) ss << n / 1000000 << "Mo";
	else if (n < 1000000000000) ss << n / 1000000000 << "Go";
	else ss << n / 1000000000000 << "To";

	return ss.str();
}
