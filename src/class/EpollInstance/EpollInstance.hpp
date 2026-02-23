#ifndef EPOLL_HPP
#define EPOLL_HPP

#include "AFd/AFd.hpp"
#include "ASocket/ASocket.hpp"

class EpollInstance : public AFd {
  private:
	EpollInstance(int epollFd);

  public:
	~EpollInstance();

	void registerFd(AFd &);
	AFd &wait() const;

	static EpollInstance create();
};

#endif
