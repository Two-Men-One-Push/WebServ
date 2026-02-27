#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "AFd/AFd.hpp"

class ASocket : public AFd {
  protected:
	ASocket(int fd);

  public:
	virtual ~ASocket();
};

#endif
