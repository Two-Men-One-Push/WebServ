#include <string>

std::string &toHeaderCase(std::string &s) {
	bool capitalize = true;
	for (std::string::iterator it = s.begin(); it != s.end(); ++it) {
		if (*it == '-')
			capitalize = true;
		else if (capitalize) {
			*it = std::toupper(*it);
			capitalize = false;
		} else {
			*it = std::tolower(*it);
		}
	}
	return s;
}
