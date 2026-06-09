#include <cctype>
#include <string>

bool ishighhexchar(char c) {
	return c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F';
}

bool islowhexchar(char c) {
	return c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f';
}

bool ishexdigit(char c) {
	return std::isdigit(c) || islowhexchar(c) || ishighhexchar(c);
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
