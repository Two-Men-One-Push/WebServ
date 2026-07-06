#include <cctype>
#include <string>

bool ishexdigit(char c) {
	return std::isdigit(c) ||
		   c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' ||
		   c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F';
}

bool ishexstr(const std::string &s) {
	for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
		if (!ishexdigit(*it)) return false;
	}
	return true;
}

bool isseparator(char c) {
	return c == '(' || c == ')' || c == '<' || c == '>' || c == '@' ||
		   c == ',' || c == ';' || c == ':' || c == '\\' || c == '"' ||
		   c == '/' || c == '[' || c == ']' || c == '?' || c == '=' ||
		   c == '{' || c == '}' || c == ' ' || c == '\t';
}

bool istokenc(char c) {
	return !(std::iscntrl(c) || isseparator(c));
}

bool istoken(const std::string &s) {
	if (s.empty()) return false;
	for (std::string::const_iterator it = s.begin(); it != s.end(); it++) {
		if (!istokenc(*it))
			return false;
	}
	return true;
}
