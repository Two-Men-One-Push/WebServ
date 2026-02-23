#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "EpollInstance/EpollInstance.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "config/MainContext/MainContext.hpp"
#include <vector>

class WebServer {
  private:
	Config _config;
	EpollInstance _epoll;
	std::vector<ListeningSocket> _listeningSockets;

  public:
 	WebServer(Config &config);
 	~WebServer();
};

#endif
