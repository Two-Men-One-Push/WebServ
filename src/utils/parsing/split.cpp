#include <cstddef>
#include <string>
#include <vector>

void split(const std::string &s, const std::string &delim, std::vector<std::string>& out) {
	std::size_t pos = 0;
	std::size_t found;

	while ((found = s.find(delim, pos)) != std::string::npos) {
		out.push_back(s.substr(pos, found - pos));
		pos = found + delim.size();
	}
	out.push_back(s.substr(pos));
}
