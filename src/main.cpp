#include <iostream>
#include <exception>
#include "renderer.h"
#include "npuzzle.h"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define EOC "\033[0m"

int		main(int ac, char** av)
{
	std::srand(std::time(nullptr));
	if (ac < 2)
	{
		std::cout << YELLOW "Usage :" EOC " ./npuzzle -g -o -k -f <filename> vstd" << std::endl\
		<< YELLOW "\t-f <filename> :" EOC " file to resolve" << std::endl\
		<< YELLOW "\t-g [size] -o -k :" EOC " use the map generator" << std::endl\
		<< YELLOW "\t -o :" EOC " generate valid map only" << std::endl\
		<< YELLOW "\t -k :" EOC " generate invalid map only" << std::endl;
	}
	else
	{
		try {
			Renderer	renderer(1280.f, 720.f);
			Npuzzle		*puzzle = new Npuzzle(ac, av);
			renderer.loop(puzzle);
			delete puzzle;
		} catch (const std::exception& e) {
			std::cout << RED << e.what() << EOC << std::endl;
		}
	}
	return (0);
}

//TO DO
//PARSING

// TO DO BONUS
//-f file Used (map) OK
//-g -generateur dans le fichier
//-v -verbose (affiche la map à chaque etat. Sinon affiche juste les moves (left, right, etc...))
//-s -solved fournir un fichier solution personnalisé
//-t -type (type algo utilisé)
//-d graphique (OK)
