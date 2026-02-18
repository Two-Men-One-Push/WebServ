#include "HttpBlock.hpp"

HttpBlock::~HttpBlock()
{}

HttpBlock::HttpBlock()
{}

HttpBlock::HttpBlock(const HttpBlock &copy)
{
	(void)copy;
}

HttpBlock	&HttpBlock::operator=(const HttpBlock &other)
{
	if (this != &other)
	{

	}
	return *this;
}