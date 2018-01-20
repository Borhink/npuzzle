#include <iostream>
#include <exception>
#include "renderer.h"
#include "npuzzle.h"

int		main(int ac, char** av)
{
	std::srand(std::time(nullptr));
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

//TO DO
//PARSING

// TO DO BONUS
//-c -cost (valeur ajoutée par move (default 1))
//-g -generateur dans le fichier
//-v -verbose (affiche la map à chaque etat. Sinon affiche juste les moves (left, right, etc...))
//-s -solved fournir un fichier solution personnalisé
//-g graphique (OK)
