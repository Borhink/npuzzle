#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "npuzzle.h"

Npuzzle::Npuzzle(char *map)
{
	_astar = nullptr;
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
	_astar = new class Astar(_solvedMap, new class Board(_board->size(), _board->getHash()));
	// _board->printMap();
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
					// std::cout << size << std::endl;
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
		return (new Board(4, "0 15 14 13 5 4 3 12 6 2 1 11 7 8 9 10"));
}
/*
return (new Board(5, "1 21 18 4 7 17 15 16 6 19 24 2 11 22 3 14 23 8 5 9 13 12 10 20 0"));
return (new Board(10, "62 1 70 33 41 11 10 44 8 39 30 34 68 81 69 7 6 47 89 13 37 2 97 86 67 73 45 4 71 50 82 5 61 93 3 40 9 42 49 96 76 77 36 38 88 35 92 98 12 46 84 63 66 56 0 53 25 15 23 14 58 57 22 16 94 83 17 65 72 80 29 28 59 64 78 51 18 90 87 21 60 55 91 99 31 48 32 85 43 20 79 26 27 95 24 54 75 52 74 19"));
return (new Board(4, "0 15 14 13 5 4 3 12 6 2 1 11 7 8 9 10"));

*/

//1 MOVE :   "1 2 3 0 8 4 7 6 5"
//8 MOVE :   "0 2 3 1 8 4 7 6 5"
//100 MOVE : "6 3 5 2 1 7 0 8 4"

// 4*4 map:
//return (new Board(4, "4 8 7 14 0 10 13 5 2 9 6 15 12 11 1 3"));
//veryhard 4*4 map : "1 12 11 10 2 13 0 9 3 14 15 8 4 5 6 7"
//hardcore 4*4 map : "0 15 14 13 5 4 3 12 6 2 1 11 7 8 9 10"

// 7*7 : "39 2 29 9 43 6 3 44 1 28 5 26 8 32 24 27 0 4 30 41 25 37 23 36 22 31 7 12 21 17 18 42 48 11 10 20 16 15 34 40 13 33 19 35 45 38 46 47 14"

// TIME
// TOTAL STATE SELECTED (closed + open)
// TOTAL STATE STORED (closed + open)
// NB MOVES
// OPENED STATE
// CLOSED STATE

/*
return (new Board(5, "1 21 18 4 7 17 15 16 6 19 24 2 11 22 3 14 23 8 5 9 13 12 10 20 0"));
PatternDatabase
Time complexity: 8600
Size complexity: 18531
Moves: 162
NO
Time complexity: 7867
Size complexity: 18311
Moves: 142


*/
