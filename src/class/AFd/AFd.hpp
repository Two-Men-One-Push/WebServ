#ifndef AFD_HPP
#define AFD_HPP

class AFd {
  protected:
	const int _fd;
	AFd(int fd);

  public:
	virtual ~AFd();
};

#endif
