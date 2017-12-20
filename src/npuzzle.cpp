#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "npuzzle.h"
#include "astar.h"

Npuzzle::Npuzzle(char *map)
{
	_board = parse(map);
	_thread = new std::thread(&Npuzzle::resolve, this);
	_thread->detach();
}

Npuzzle::~Npuzzle()
{
	delete _thread;
	delete _board;
	_solvedMap.clear();
}

void	Npuzzle::resolve(void)
{
	_solvedMap.resize(_board->size() * _board->size(), glm::ivec2(0, 0));
	_board->getSolvedPoints(_solvedMap);
	class Astar astar(_solvedMap, new class Board(_board->size(), _board->getHash()));
	// class Astar astar(_solvedMap, _board);
	_board->printMap();
}

Board	*Npuzzle::parse(char *path)
{
	size_t	size = 0;
	std::ifstream fileStream(path, std::ios::in);
	std::string lineMap("");
	if(fileStream.is_open()){
		std::string line = "";
		while(getline(fileStream, line))
		{
			std::size_t found = line.find("#");
			if (found > 0)
			{
				std::string str = line.substr(0, found);
				if (!size)
				{
					size = std::stoi(str);
					std::cout << size << std::endl;
				}
				else
				{

				}
			}
		}
		fileStream.close();
	}
	else
		throw std::logic_error("Impossible to open map\n");
	return (new Board(3, "6 3 5 2 1 7 0 8 4"));
}

//1 MOVE : "1 2 3 0 8 4 7 6 5"
//8 MOVE : "0 2 3 1 8 4 7 6 5"
