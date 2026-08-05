#ifndef FILE_HPP
#define FILE_HPP

#include "Fd/Fd.hpp"
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

class File : public Fd {
  private:
	File(const File &other);
	File &operator=(const File &other);

  protected:
	File(int fd);
	static int openFd(const std::string &path, int flags, mode_t mode = 0);

  public:
	File(const std::string &path, int flags, mode_t mode = 0);
	virtual ~File();

};

#endif
