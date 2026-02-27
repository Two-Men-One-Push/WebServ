#include "MainContext.hpp"
#include <exception>
#include <fstream>
#include <iostream>

int	main(int argc, char **argv)
{
	if (argc <= 1)
		return (0);
	std::ifstream	stream(argv[1]);
	if (!stream.is_open())
		return (1);
	try {
	{
		MainContext	config(stream);
	}
	} catch (const std::invalid_argument &err)
	{
		std::cout << err.what() << std::endl;
	}
	return (0);
}
