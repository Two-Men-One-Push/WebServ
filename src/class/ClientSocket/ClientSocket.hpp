#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include "ASocket/ASocket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

class	ClientSocket: ASocket
{
	private:
		struct sockaddr_storage	_address;
		socklen_t				_addressLen;
	public:
		~ClientSocket();
		ClientSocket(int fd, struct sockaddr_storage &_address, socklen_t _addressLen);

		const struct sockaddr_storage &getAdress() const;
		socklen_t getAdressLen() const;
};

#endif
