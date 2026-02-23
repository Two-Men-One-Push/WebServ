#include "MainContext.hpp"

int	main(int argc, char **argv)
{
	if (argc <= 1)
		return (0);
	MainContext	config(argv[1]);
}
