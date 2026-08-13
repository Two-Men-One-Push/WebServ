#ifndef BODYTMPFILE_HPP
#define BODYTMPFILE_HPP

#include "File/TmpFile.hpp"
#include "http/messages/Body/IBody.hpp"
#include <string>
#include <sys/types.h>
#include <unistd.h>

class BodyTmpFile : public TmpFile, public IBody {
  private:
	BodyTmpFile(const BodyTmpFile &other);
	BodyTmpFile operator=(const BodyTmpFile &other);

  public:
	BodyTmpFile(const std::string &path, int flags, mode_t mode = 0);
	~BodyTmpFile();

	ssize_t read(char *buf, size_t size);
	ssize_t write(const char *buf, size_t size);
};

#endif
