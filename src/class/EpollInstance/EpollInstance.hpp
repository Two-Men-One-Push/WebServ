#ifndef EPOLL_HPP
#define EPOLL_HPP

#include "AFd/AFd.hpp"
#include <sys/types.h>
#include <vector>

struct EpollEvent {
	u_int32_t events;
	AFd *fd;
};

class EpollInstance : public AFd {
  private:
	EpollInstance(int epollFd);

  public:
	~EpollInstance();

	void registerFd(AFd &) const;
	void updateFd(AFd &fd) const;
	void wait(std::vector<EpollEvent> &result) const;

	void handleEvents(u_int32_t events, WebServer &webServer) { (void)events; (void)webServer; }
	u_int32_t getHandledEvents() const { return 0; }

	static EpollInstance create();
};

#endif
