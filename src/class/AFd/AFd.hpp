#ifndef AFD_HPP
#define AFD_HPP

#include <string>
#include <sys/types.h>
#include <unistd.h>

class WebServer;

class AFd {
  protected:
	const int _fd;
	AFd(int fd);

  public:
	int fd() const;

	ssize_t write(const void *buf, size_t n);
	ssize_t send(const void* buf, size_t len, int flags = 0);

	virtual void handleEvents(u_int32_t events, WebServer &webServer) = 0;
	virtual u_int32_t getHandledEvents() const = 0;

	virtual ~AFd();
};

#endif
