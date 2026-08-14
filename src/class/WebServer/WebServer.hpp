#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "CGI/CGIInterface.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "EpollInstance/EpollInstance.hpp"
#include "Fd/Fd.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "model/Config/Config.hpp"
#include <csignal>
#include <vector>

class WebServer {
  private:
	EpollInstance _epoll;

	std::vector<ListeningSocket *> _listeningSockets;
	std::vector<ClientSocket *> _clientSockets;
	std::vector<CGIInterface *> _cgiInterfaces;
	std::vector<ClientSocket *> _clientSocketsToDelete;
	std::vector<CGIInterface *> _cgiInterfacesToDelete;

	void startListeningSockets(const Config &config);
	void deleteClientSockets();
	void deleteCGIInterfaces();

	static sig_atomic_t running;
	static sig_atomic_t signal;
	static void sigintHandler(int);

  public:
	WebServer(const Config &config);
	~WebServer();

	void addClient(ClientSocket *client);
	void requestDeleteClient(ClientSocket *client);
	void addCGIInterface(CGIInterface *interface);
	void requestDeleteCGIInterface(CGIInterface *interface);
	const EpollInstance &epoll() const;
};

#endif
