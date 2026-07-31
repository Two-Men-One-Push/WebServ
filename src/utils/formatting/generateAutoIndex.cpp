#include "Directory/Directory.hpp"
#include "errors/WebservErrors.hpp"
#include "utils/formatting.hpp"
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <ostream>
#include <string>


void printFileLine(std::ostream &os, const std::string &file, const std::string &parentDirectory) {
	struct stat st;
	char buf[32];

	if (::stat((parentDirectory + '/' + file).c_str(), &st)) throw WebservErrors::SysError("stat", errno);

	std::tm* tm = std::localtime(&st.st_mtime);
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", tm);

	os << "<tr>\
<td></td>\
<td><a href=\"./" << file << "\">" << file << "</a></td>\
<td>" << buf << "</td>\
<td align=\"right\">" << formatSize(st.st_size) << "</td>\
</tr>";
}

void printDirectoryLine(std::ostream &os, const std::string &directory, const std::string &parentDirectory) {
	struct stat st;
	char buf[32];

	if (::stat((parentDirectory + '/' + directory).c_str(), &st)) throw WebservErrors::SysError("stat", errno);

	std::tm* tm = std::localtime(&st.st_mtime);
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", tm);

	os << "<tr>\
<td>📁</td>\
<td><a href=\"" << directory << "/\">" << directory << "</a></td>\
<td>" << buf << "</td>\
<td align=\"right\">-</td>\
</tr>";
}

void printAutoIndex(std::ostream &os, const std::string &directoryPath) {
	Directory dir(directoryPath.c_str());

	os << "<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
<meta charset=\"UTF-8\">\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
<title>Index of " << directoryPath << "</title>\
</head>\
<body>\
<h1>" << directoryPath << "</h1>\
<hr/>";
	if (true) os << "<p><a href=\"..\">⬅ Parent Directory</p>";
	os << "<table>\
<thead>\
<tr>\
<th align=\"center\">Type</th>\
<th align=\"left\">Name</th>\
<th align=\"left\">Last Modified</th>\
<th align=\"right\">Size</th>\
</tr>\
</thead>\
<tbody>";
	for (dirent *entry = dir.read(); entry != NULL; entry = dir.read()) {
		const std::string entryName(entry->d_name);

		if (entryName == "." || entryName == ".." || (entry->d_type != DT_DIR && entry->d_type != DT_REG)) continue;

		if (entry->d_type == DT_REG) printFileLine(os, entryName, directoryPath);
		if (entry->d_type == DT_DIR) printDirectoryLine(os, entryName, directoryPath);
	}

	os << "</tbody>\
</table>\
</body>\
</html>";
}
