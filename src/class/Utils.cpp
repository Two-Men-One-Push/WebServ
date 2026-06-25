#include <sstream>
#include <string>

bool	parseInt(const std::string &str, int &out)
{
	std::istringstream	iss(str);
	int	value;
	iss >> value;
	if (iss.fail() || !iss.eof())
		return false;
	out = value;
	return true;
}