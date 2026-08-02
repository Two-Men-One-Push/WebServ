#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <dirent.h>
#include <string>
class Directory {
	private:
		Directory(const Directory &other);
		Directory &operator=(const Directory &other);
		DIR *dir;
	public:
		Directory(const std::string &dirPath);
		~Directory();

		dirent *read();
};

#endif
