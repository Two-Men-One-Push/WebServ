#include "ServerBlock.hpp"

ServerBlock::~ServerBlock()
{}

ServerBlock::ServerBlock()
{}

ServerBlock::ServerBlock(const ServerBlock &copy)
{
	(void)copy;
}

ServerBlock	&ServerBlock::operator=(const ServerBlock &other)
{
	if (this != &other)
	{

	}
	return *this;
}