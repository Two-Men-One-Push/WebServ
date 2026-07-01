#include <sstream>
#include <string>
#include <vector>

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
std::string	trim_path(const std::string &path)
{
	std::string result = path;
	while (!result.empty() && result[0] == '/')
		result.erase(0, 1);
	while (!result.empty() && result[result.size() - 1] == '/')
		result.erase(result.size() - 1, 1);
	return result;
}

std::string	pathJoin(const std::string &path1, const std::string &path2)
{
	std::string	result;
	result = "/";
	if (!trim_path(path1).empty())
		result += trim_path(path1) + "/";
	if (!trim_path(path2).empty())
		result += trim_path(path2);
	return result;
}

bool	pathNormalize(std::string &result, const std::string &path)
{
	bool	error = false;
	std::string tmp = path;
	while (!tmp.empty() && tmp[0] == '/')
		tmp.erase(0, 1);
	while (!tmp.empty() && tmp[tmp.size() - 1] == '/')
		tmp.erase(tmp.size() - 1, 1);
	std::vector<std::string> parts;
	std::string::size_type start = 0;
	while (start < tmp.size())
	{
		std::string::size_type end = tmp.find('/', start);
		if (end == std::string::npos)
			end = tmp.size();
		std::string part = tmp.substr(start, end - start);
		if (part == "..")
		{
			if (!parts.empty())
				parts.pop_back();
			else
				error = true;
		}
		else if (!part.empty() && part != ".")
			parts.push_back(part);
		start = end + 1;
	}
	result = "/";
	for (std::vector<std::string>::const_iterator it = parts.begin(); it != parts.end(); ++it)
	{
		if (it != parts.begin())
			result += "/";
		result += *it;
	}
	return error;
}