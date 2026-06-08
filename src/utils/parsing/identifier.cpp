#include <cctype>
#include <string>

bool ishighhchar(char c) {
	return c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F';
}

bool islowhchar(char c) {
	return c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f';
}

bool ishex(char c) {
	return std::isdigit(c) || islowhchar(c) || ishighhchar(c);
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
