#include <iostream>
#include <exception>
#include "renderer.h"
#include "npuzzle.h"

int		main(int ac, char** av)
{
	if (ac < 2)
		std::cout << "Usage: ./npuzzle [map file]" << std::endl;
	else
	{
		try {
			Renderer	renderer(1280.f, 720.f);
			Npuzzle		*puzzle = new Npuzzle(av[1]);
			renderer.loop(puzzle);
			delete puzzle;
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	return (0);
}
