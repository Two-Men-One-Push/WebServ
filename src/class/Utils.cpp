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

bool	parseSize(const std::string &str, size_t &out)
{
	size_t i = 0;
	while (i < str.size() && std::isdigit(static_cast<unsigned char>(str[i])))
	    i++;
	std::string number = str.substr(0, i);
	std::string suffix = str.substr(i);
	for (size_t j = 0; j < suffix.size(); ++j)
		suffix[j] = std::tolower(static_cast<unsigned char>(suffix[j]));
	std::istringstream	iss(number);
	size_t	value;
	iss >> value;
	if (iss.fail() || !iss.eof())
		return false;
	if (suffix.empty())
		out = value;
	else if (suffix == "k" || suffix == "ko" || suffix == "kb")
		out = value * 1000;
	else if (suffix == "m" || suffix == "mo" || suffix == "mb")
		out = value * 1000 * 1000;
	else if (suffix == "g" || suffix == "go" || suffix == "gb")
		out = value * 1000 * 1000 * 1000;
	else if (suffix == "ki" || suffix == "kio" || suffix == "kib")
		out = value * 1024;
	else if (suffix == "mi" || suffix == "mio" || suffix == "mib")
		out = value * 1024 * 1024;
	else if (suffix == "gi" || suffix == "gio" || suffix == "gib")
		out = value * 1024 * 1024 * 1024;
	else if (!suffix.empty())
	{
		out = value;
		return false;
	}
	return true;
}