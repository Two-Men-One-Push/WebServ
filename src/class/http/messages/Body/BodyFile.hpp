#ifndef BODYFILE_HPP
#define BODYFILE_HPP

#include "File/File.hpp"
#include "http/messages/Body/IBody.hpp"
#include <string>
#include <sys/types.h>
#include <unistd.h>

class BodyFile : public File, public IBody {
  private:
	BodyFile(const BodyFile &other);
	BodyFile operator=(const BodyFile &other);

  public:
	BodyFile(const std::string &path, int flags, mode_t mode = 0);
	~BodyFile();

	ssize_t read(char *buf, size_t size);
	ssize_t write(const char *buf, size_t size);
};

#endif
