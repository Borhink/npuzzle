#include <iostream>
#include "npuzzle.h"

int		main(int ac, char** av)
{
	if (ac < 2)
		std::cout << "Usage: ./npuzzle [map file]" << std::endl;
	else
	{
		class Npuzzle	npuzzle(av[1]);
	}
	return (0);
}
