#ifndef AFD_HPP
#define AFD_HPP

#include <sys/types.h>

class AFd {
  protected:
	const int _fd;
	AFd(int fd);

  public:

  	int getFd() const;

	virtual ~AFd();
};

#endif
