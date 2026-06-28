#ifndef EPOLL_HPP
#define EPOLL_HPP

#include "AFd/AFd.hpp"
#include <sys/types.h>
#include <vector>

class AEpollWatchable;

struct EpollEvent {
	u_int32_t events;
	AEpollWatchable *fd;
};

class EpollInstance : public AFd {
  private:
	EpollInstance(int epollFd);

  public:
	~EpollInstance();

	void add(AEpollWatchable &fd) const;
	void mod(AEpollWatchable &fd) const;
	void del(int fd) const;
	void del(AEpollWatchable &fd) const;
	void wait(std::vector<EpollEvent> &result) const;

	static EpollInstance create();
};

#endif
