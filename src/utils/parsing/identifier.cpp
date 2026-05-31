#include <cctype>
#include <string>

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
	for (std::string::const_iterator it = s.begin(); it != s.end(); it++) {
		if (!istokenc(*it))
			return false;
	}
	return true;
}
