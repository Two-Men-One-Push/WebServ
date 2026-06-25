#ifndef AFD_HPP
#define AFD_HPP

#include <sys/types.h>
#include <unistd.h>

class WebServer;

class AFd {
  protected:
	const int _fd;
	AFd(int fd);

  public:
	int fd() const;

	ssize_t read(void *buf, size_t n) const;
	ssize_t recv(void *buf, size_t n, int flags = 0) const;

	ssize_t write(const void *buf, size_t n) const;
	ssize_t send(const void *buf, size_t len, int flags = 0) const;

	int dup2(int fd) const;

	virtual ~AFd();
};

#endif
