#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "AFd/AFd.hpp"

class ASocket : public AFd {
  protected:
	virtual ~ASocket();

  public:
	ASocket(int fd);
	static ASocket createSocket();
};

#endif
