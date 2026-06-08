#include "utils/parsing.hpp"
#include <cctype>
#include <stdexcept>
#include <string>

static char htoc(char c) {
	if (c >= 'a') return c - 'a';
	if (c >= 'A') return c - 'A';
	return c - '0';
}

std::string uriDecode(const std::string &s) {
	std::string result;

	result.reserve(s.size());
	for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
		if (*it == '%') {
			char fromHexByte = 0;
			++it;
			if (it == s.end()) throw std::invalid_argument("Unexpected input end");
			if (!ishex(*it)) throw std::invalid_argument("Expected hexadecimal character");
			fromHexByte += htoc(*it) * 16;
			++it;
			if (it == s.end()) throw std::invalid_argument("Unexpected input end");
			if (!ishex(*it)) throw std::invalid_argument("Expected hexadecimal character");
			fromHexByte += htoc(*it);
			if (fromHexByte == '\0') throw std::invalid_argument("Unexpected encoded null byte");
			result += fromHexByte;
		} else {
			result += *it;
		}
	}
	return result;
}
