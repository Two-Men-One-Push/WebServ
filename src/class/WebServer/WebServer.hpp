#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "ClientSocket/ClientSocket.hpp"
#include "EpollInstance/EpollInstance.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "config/MainContext/MainContext.hpp"
#include <vector>

class WebServer {
  private:
	Config _config;
	EpollInstance _epoll;
	std::vector<ListeningSocket *> _listeningSockets;
	std::vector<ClientSocket *> _clientSockets;

  public:
	WebServer(Config &config);
	void addClient(ClientSocket *client);
	~WebServer();
};

#endif
