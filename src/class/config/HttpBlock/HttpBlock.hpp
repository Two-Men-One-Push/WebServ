#ifndef HTTP_BLOCK_HPP
#define HTTP_BLOCK_HPP

class	HttpBlock
{
	private:
	public:
		~HttpBlock();
		HttpBlock();
		HttpBlock(const HttpBlock &copy);
		HttpBlock	&operator=(const HttpBlock &other);
};

#endif