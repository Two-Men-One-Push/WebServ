#ifndef EPOLL_HPP
#define EPOLL_HPP

#include "AFd/AFd.hpp"

class Epoll : public AFd {
  private:
	static int createEpoll();

  public:
	Epoll();
	~Epoll();
};

#endif
