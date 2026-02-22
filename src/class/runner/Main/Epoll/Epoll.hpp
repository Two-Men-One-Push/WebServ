#ifndef EPOLL_HPP
#define EPOLL_HPP

#include "AFd/AFd.hpp"

class Epoll : public AFd {
  private:
	Epoll(int epollFd);

  public:
	~Epoll();

	static Epoll create();
};

#endif
