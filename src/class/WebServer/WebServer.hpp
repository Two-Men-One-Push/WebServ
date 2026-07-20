#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "ClientSocket/ClientSocket.hpp"
#include "EpollInstance/EpollInstance.hpp"
#include "Fd/Fd.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "model/Config/Config.hpp"
#include <vector>

class WebServer {
  private:
	EpollInstance _epoll;

	std::vector<ListeningSocket *> _listeningSockets;
	std::vector<ClientSocket *> _clientSockets;
	std::vector<ClientSocket *> _clientSocketsToDelete;

	void deleteClientSockets();

	void startListeningSockets(const Config &config);

  public:
	WebServer(const Config &config);
	~WebServer();

	void addClient(ClientSocket *client);
	void requestDelete(ClientSocket *client);
	const EpollInstance &epoll() const;
};

#endif
