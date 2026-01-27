#ifndef SOCKET_HPP
#define SOCKET_HPP

class ASocket
{
	protected:
		const int	_fd;
		ASocket(int fd);
	public:
		virtual ~ASocket();
};

#endif