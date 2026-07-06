#ifndef EPOLL_HPP
#define EPOLL_HPP

#include "Fd/Fd.hpp"
#include <sys/types.h>
#include <vector>

class AEpollWatchable;

struct EpollEvent {
	u_int32_t events;
	AEpollWatchable *fd;
};

class EpollInstance : public Fd {
  private:
	static int createEpollFd();
	EpollInstance(const EpollInstance &other);
	EpollInstance &operator=(const EpollInstance &other);

  public:
	EpollInstance();
	~EpollInstance();

	void add(AEpollWatchable &fd) const;
	void mod(AEpollWatchable &fd) const;
	void del(int fd) const;
	void del(AEpollWatchable &fd) const;
	void wait(std::vector<EpollEvent> &result) const;
};

#endif
