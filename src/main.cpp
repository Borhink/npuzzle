#include <iostream>
#include <exception>
#include "renderer.h"
#include "npuzzle.h"

int		main(int ac, char** av)
{
	std::srand(std::time(nullptr));
	if (ac < 2)
	{
		std::cout << YELLOW "Usage :" EOC " ./npuzzle [-fg] -okhvdcs" << std::endl\
		<< YELLOW "\t-f [filename] :" EOC " file to resolve" << std::endl\
		<< YELLOW "\t-g [size] :" EOC " use the map generator" << std::endl\
		<< YELLOW "\t\t-o :" EOC " generate valid map only" << std::endl\
		<< YELLOW "\t\t-k :" EOC " generate invalid map only" << std::endl\
		<< YELLOW "\t-h [type] :" EOC " heuristic used. (ex : -h 3 = Manhattan + Linear Conflict)" << std::endl\
		<< YELLOW "\t\t1  -> " EOC "Manhattan" << std::endl\
		<< YELLOW "\t\t2  -> " EOC "Linear Conflict" << std::endl\
		<< YELLOW "\t\t4  -> " EOC "Out of Row and Column" << std::endl\
		<< YELLOW "\t\t8  -> " EOC "Euclidean" << std::endl\
		<< YELLOW "\t\t16 -> " EOC "Misplaced" << std::endl\
		<< YELLOW "\t\t32 -> " EOC "Dijkstra (cant be combined)" << std::endl\
		<< YELLOW "\t-v :" EOC " verbose print" << std::endl\
		<< YELLOW "\t-d :" EOC " render displayed with openGL" << std::endl\
		<< YELLOW "\t-c :" EOC " check move validity at end" << std::endl\
		<< YELLOW "\t-s [filename] :" EOC " custom solution for the map" << std::endl;
	}
	else
	{
		try {
			Npuzzle		*puzzle = new Npuzzle(ac, av);
			if (puzzle->getDisplayGL())
			{
				Renderer	renderer(1280.f, 720.f);
				renderer.loop(puzzle);
			}
			delete puzzle;
		} catch (const std::exception& e) {
			std::cout << RED << e.what() << EOC << std::endl;
		}
	}
	return (0);
}

// TO DO BONUS
//-s -solved fournir un fichier solution personnalisé
