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
	_solvedMap.resize(_board->size() * _board->size(), glm::ivec2(0, 0));
	_board->getSolvedPoints(_solvedMap);
	if (this->checkIfSolvable())
	{
		_thread = new std::thread(&Npuzzle::resolve, this);
		_thread->detach();
	}
	else
		std::cout << "Error: map is unsolvable" << std::endl;
}

Npuzzle::~Npuzzle()
{
	delete _thread;
	delete _board;
	_solvedMap.clear();
}

int	Npuzzle::countInversions(int x, int y, std::vector<std::vector<int>> &map)
{
	int nb = map[y][x];
	int inversions = 0;
	int k = x + y * _board->size();

	for (; k < _board->size() * _board->size(); k++)
	{
		x = k % _board->size();
		y = k / _board->size();
		if (map[y][x] && map[y][x] < nb)
			inversions++;
	}
	return (inversions);
}

bool	Npuzzle::checkIfSolvable(void)
{
	std::vector<std::vector<int>> map = _board->getMap();
	int startInversions = 0, endInversions = 0;
	int startPosY = 0, endPosY = 0;

	for (int y = 0; y < _board->size(); y++)
	{
		for (int x = 0; x < _board->size(); x++)
		{
			if (map[y][x])
				startInversions += this->countInversions(x, y, map);
			else
				startPosY = y;
		}
	}
	for (int i = 0; i < _board->size() * _board->size(); i++)
		map[_solvedMap[i][Y]][_solvedMap[i][X]] = i;
	for (int y = 0; y < _board->size(); y++)
	{
		for (int x = 0; x < _board->size(); x++)
		{
			if (map[y][x])
				endInversions += this->countInversions(x, y, map);
			else
				endPosY = y;
		}
	}
	if (!(_board->size() % 2))
	{
		startInversions += startPosY;
		endInversions += endPosY;
	}
	return ((startInversions % 2) == (endInversions % 2));
}

void	Npuzzle::resolve(void)
{
	_astar = new class Astar(_solvedMap, new class Board(_board->size(), _board->getHash()));
	// _board->printMap();
}

class Board	*Npuzzle::parse(char *path)
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
	// return (new Board(3, "6 3 5 2 1 7 0 8 4"));
	// return (new Board(4, "0 15 14 13 5 4 3 12 6 2 1 11 7 8 9 10"));
	// return (new Board(5, "1 21 18 4 7 17 15 16 6 19 24 2 11 22 3 14 23 8 5 9 13 12 10 20 0"));
	return (new Board(7, "43 37 24 35 14 48 40 17 22 47 1 8 11 10 15 36 25 4 28 26 18 42 0 16 46 3 41 7 30 6 5 38 33 20 9 29 13 23 21 27 19 45 31 34 2 39 44 12 32"));
	// return (new Board(7, "42 46 7 23 40 15 38 5 44 43 13 45 33 41 9 48 37 26 47 18 31 32 39 34 35 11 2 27 16 1 12 29 21 36 19 24 10 17 28 25 3 4 14 8 20 22 0 30 6"));
	// return (new Board(7, "39 2 29 9 43 6 3 44 1 28 5 26 8 32 24 27 0 4 30 41 25 37 23 36 22 31 7 12 21 17 18 42 48 11 10 20 16 15 34 40 13 33 19 35 45 38 46 47 14"));
	// return (new Board(10, "62 1 70 33 41 11 10 44 8 39 30 34 68 81 69 7 6 47 89 13 37 2 97 86 67 73 45 4 71 50 82 5 61 93 3 40 9 42 49 96 76 77 36 38 88 35 92 98 12 46 84 63 66 56 0 53 25 15 23 14 58 57 22 16 94 83 17 65 72 80 29 28 59 64 78 51 18 90 87 21 60 55 91 99 31 48 32 85 43 20 79 26 27 95 24 54 75 52 74 19"));
}
