#ifndef BODYSTRINGSTREAM_HPP
#define BODYSTRINGSTREAM_HPP

#include "http/messages/Body/IBody.hpp"
#include <sstream>
#include <sys/types.h>
#include <unistd.h>

class BodyStringStream : public std::stringstream, public IBody {
  private:
	BodyStringStream(const BodyStringStream &other);
	BodyStringStream operator=(const BodyStringStream &other);

  public:
	BodyStringStream();

	~BodyStringStream();

	ssize_t read(char *buf, size_t size);
	ssize_t write(const char *buf, size_t size);
};

#endif
