#ifndef IBODY_HPP
#define IBODY_HPP

#include <cstddef>
#include <sys/types.h>

class IBody {
  public:
	virtual ~IBody() {}

	virtual ssize_t read(char *buf, size_t size) = 0;
	virtual ssize_t write(const char *buf, size_t size) = 0;
};

#endif
