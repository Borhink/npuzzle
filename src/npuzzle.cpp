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
	return (new Board(7, "39 2 29 9 43 6 3 44 1 28 5 26 8 32 24 27 0 4 30 41 25 37 23 36 22 31 7 12 21 17 18 42 48 11 10 20 16 15 34 40 13 33 19 35 45 38 46 47 14"));
}

//1 MOVE :   "1 2 3 0 8 4 7 6 5"
//8 MOVE :   "0 2 3 1 8 4 7 6 5"
//100 MOVE : "6 3 5 2 1 7 0 8 4"

// 4*4 map : "4 8 7 14 0 10 13 5 2 9 6 15 12 11 1 3"
//veryhard 4*4 map : "1 12 11 10 2 13 0 9 3 14 15 8 4 5 6 7"
//hardcore 4*4 map : "0 15 14 13 5 4 3 12 6 2 1 11 7 8 9 10"

// TIME
// TOTAL STATE SELECTED (closed + open)
// TOTAL STATE STORED (closed + open)
// NB MOVES
// OPENED STATE
// CLOSED STATE


// EASY 4MAP, for testing Dijkstra : "3 14 4 5 2 12 0 15 13 1 11 6 10 9 8 7"
// 180 sec...
// Time complexity: 2.556.113
// Size complexity: 4.887.627
// Moves: 20

// Time complexity: 152
// Size complexity: 322
// Moves: 30
