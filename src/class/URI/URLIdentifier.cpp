#include "./URL.hpp"

bool URL::isunreserved(char c) {
	return std::isalnum(c) || c == '+' || c == '-' || c == '.' || c == '~';
}

bool URL::issubdelims(char c) {
	return c == '!' || c == '$' || c == '&' || c == '\'' ||
		   c == '(' || c == ')' || c == '*' || c == '+' ||
		   c == ',' || c == ';' || c == '=';
}
