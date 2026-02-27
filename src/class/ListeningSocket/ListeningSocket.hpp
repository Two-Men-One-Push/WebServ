#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "ASocket.hpp"
#include "ClientSocket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

class	ListeningSocket: ASocket
{
	private:
	public:
		~ListeningSocket();
		ListeningSocket(const struct sockaddr_storage &address);

		ClientSocket *acceptConnexion(void) const;
		int	getFd(void) const;
};

#endif
