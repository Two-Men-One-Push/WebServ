#ifndef AFD_HPP
#define AFD_HPP

#include <sys/types.h>

class WebServer;

class AFd {
  protected:
	const int _fd;
	AFd(int fd);

  public:
	int getFd() const;

	virtual void handleEvents(u_int32_t events, WebServer &webServer) = 0;
	virtual u_int32_t getHandledEvents() const = 0;

	virtual ~AFd();
};

#endif
