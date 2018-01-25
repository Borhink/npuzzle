#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "npuzzle.h"

Npuzzle::Npuzzle(int ac, char **av)
: _board(NULL), _generate(false), _generateParam(0)
{
	_astar = nullptr;
	this->parseArgs(ac, av);
	if (_generate)
		_board = this->generate();
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
}

int	Npuzzle::parseArgs(int ac, char **av)
{
	for (int i = 1; i < ac; i++)
	{
		if (std::string(av[i]) == "-f" && i + 1 < ac && !_board && !_generate)
			_board = this->parse(av[++i]);
		else if (std::string(av[i]) == "-g" && i + 1 < ac && !_board && !_generate)
		{
			_generateSize = atoi(av[++i]);
			if (_generateSize >= 3)
				_generate = true;
		}
		else if (std::string(av[i]) == "-o")
			_generateParam = VALID;
		else if (std::string(av[i]) == "-k")
			_generateParam = INVALID;
	}
	if (!_board && !_generate)
		throw std::logic_error("No map to solve");
	return (1);
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
		throw std::logic_error("Impossible to open map");
	return (new Board(3, "6 3 5 2 1 7 0 8 4"));
	// return (new Board(4, "0 15 14 13 5 4 3 12 6 2 1 11 7 8 9 10"));
	// return (new Board(5, "1 21 18 4 7 17 15 16 6 19 24 2 11 22 3 14 23 8 5 9 13 12 10 20 0"));
	// return (new Board(7, "43 37 24 35 14 48 40 17 22 47 1 8 11 10 15 36 25 4 28 26 18 42 0 16 46 3 41 7 30 6 5 38 33 20 9 29 13 23 21 27 19 45 31 34 2 39 44 12 32"));
	// return (new Board(7, "42 46 7 23 40 15 38 5 44 43 13 45 33 41 9 48 37 26 47 18 31 32 39 34 35 11 2 27 16 1 12 29 21 36 19 24 10 17 28 25 3 4 14 8 20 22 0 30 6"));
	// return (new Board(7, "39 2 29 9 43 6 3 44 1 28 5 26 8 32 24 27 0 4 30 41 25 37 23 36 22 31 7 12 21 17 18 42 48 11 10 20 16 15 34 40 13 33 19 35 45 38 46 47 14"));
	// return (new Board(10, "62 1 70 33 41 11 10 44 8 39 30 34 68 81 69 7 6 47 89 13 37 2 97 86 67 73 45 4 71 50 82 5 61 93 3 40 9 42 49 96 76 77 36 38 88 35 92 98 12 46 84 63 66 56 0 53 25 15 23 14 58 57 22 16 94 83 17 65 72 80 29 28 59 64 78 51 18 90 87 21 60 55 91 99 31 48 32 85 43 20 79 26 27 95 24 54 75 52 74 19"));
}

class Board *Npuzzle::generate(void)
{
	std::vector<std::vector<int>> map;
	int iteration = _generateSize * _generateSize << 4;
	int dir;
	int x, y;

	map.resize(_generateSize);
	for (int y = 0; y < _generateSize; y++)
		map[y].resize(_generateSize, 0);
	this->generateVectorMap(map, _generateSize, 0, 0, RIGHT, 1, 1);
	for (int tmpY = 0; tmpY < _generateSize; tmpY++)
		for (int tmpX = 0; tmpX < _generateSize; tmpX++)
			if (map[tmpY][tmpX] == 0)
			{
				x = tmpX;
				y = tmpY;
			}
	if (_generateParam == INVALID || (!_generateParam && rand() % 2))
	{
		bool isSwap = false;
		for (int tmpY = 0; tmpY < _generateSize && !isSwap; tmpY++)
			for (int tmpX = 0; tmpX < _generateSize && !isSwap; tmpX++)
				if (tmpX + 1 < _generateSize && map[tmpY][tmpX] != 0 && map[tmpY][tmpX + 1] != 0)
				{
					std::swap(map[tmpY][tmpX], map[tmpY][tmpX + 1]);
					isSwap = true;
				}
	}
	for (int i = 0; i < iteration; i++)
	{
		dir = (1 + rand() % 4) * 2;
		if (dir == LEFT && x > 0)
		{
			std::swap(map[y][x], map[y][x - 1]);
			x--;
		}
		if (dir == RIGHT && x < _generateSize - 1)
		{
			std::swap(map[y][x], map[y][x + 1]);
			x++;
		}
		if (dir == UP && y > 0)
		{
			std::swap(map[y][x], map[y - 1][x]);
			y--;
		}
		if (dir == DOWN && y < _generateSize - 1)
		{
			std::swap(map[y][x], map[y + 1][x]);
			y++;
		}
	}
	return (new Board(_generateSize, map));
}

void Npuzzle::generateVectorMap(std::vector<std::vector<int>> &map, int len, int x, int y, int dir, int nb, int swap)
{
	int	i = 0;

	if (len == 0)
		return;
	while (i < len)
	{
		map[y][x] = nb % (_generateSize * _generateSize);
		dir >= LEFT ? dir >= RIGHT ? dir >= UP ? y-- : x++ : x-- : y++;
		nb++;
		i++;
	}
	swap = (swap + 1) % 2;
	if (swap == 0) len--;
	if (dir == RIGHT)
		this->generateVectorMap(map, len, x - 1, y + 1, DOWN, nb, swap);
	if (dir == DOWN)
		this->generateVectorMap(map, len, x - 1, y - 1, LEFT, nb, swap);
	if (dir == LEFT)
		this->generateVectorMap(map, len, x + 1, y - 1, UP, nb, swap);
	if (dir == UP)
		this->generateVectorMap(map, len, x + 1, y + 1, RIGHT, nb, swap);
}
