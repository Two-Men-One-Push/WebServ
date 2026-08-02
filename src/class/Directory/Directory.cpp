#include "./Directory.hpp"
#include "errors/WebservErrors.hpp"
#include <dirent.h>

Directory::Directory(const std::string &dirPath) : dir(opendir(dirPath.c_str())) {
	if (!this->dir) throw WebservErrors::SysError("opendir", errno);
}

Directory::~Directory() {
	closedir(this->dir);
}

dirent *Directory::read() {
	return readdir(dir);
}
