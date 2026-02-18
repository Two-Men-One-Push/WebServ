#ifndef SERVER_BLOCK_HPP
#define SERVER_BLOCK_HPP

class	ServerBlock
{
	private:
	public:
		~ServerBlock();
		ServerBlock();
		ServerBlock(const ServerBlock &copy);
		ServerBlock	&operator=(const ServerBlock &other);
};

#endif