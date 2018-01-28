#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "npuzzle.h"

Npuzzle::Npuzzle(int ac, char **av)
: _board(NULL), _generate(false), _generateParam(0),
_heuristicParam(MANHATTAN | LINEAR_CONFLICT), _verboseParam(false),
_checkParam(false), _displayGL(false)
{
	_astar = nullptr;
	this->parseArgs(ac, av);
	if (_generate)
		_board = this->generate();
	if (!_solvedMap.size())
	{
		_solvedMap.resize(_board->size() * _board->size(), glm::ivec2(0, 0));
		_board->getSolvedPoints(_solvedMap);
	}
	if (_solvedMap.size() != _board->size() * _board->size())
		throw std::logic_error("Error: map and custom solution have a different size");
	if (this->checkIfSolvable())
	{
		if (_displayGL)
		{
			_thread = new std::thread(&Npuzzle::resolve, this);
			_thread->detach();
		}
		else
			this->resolve();
	}
	else
		throw std::logic_error("Error: map is unsolvable");
}

Npuzzle::~Npuzzle()
{
	if (_displayGL)
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
	_astar = new class Astar(_solvedMap, new class Board(_board->size(), _board->getHash()), _heuristicParam, _verboseParam, _checkParam);
}

int	Npuzzle::parseArgs(int ac, char **av)
{
	int nb = 0;
	class Board *tmp = NULL;

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
		else if (std::string(av[i]) == "-h" && i + 1 < ac)
		{
			nb = atoi(av[++i]);
			if (nb >= 0 && nb < HEURISTIC_COUNT)
				_heuristicParam = nb;
		}
		else if (std::string(av[i]) == "-v")
			_verboseParam = true;
		else if (std::string(av[i]) == "-c")
			_checkParam = true;
		else if (std::string(av[i]) == "-d")
			_displayGL = true;
		else if (std::string(av[i]) == "-s" && i + 1 < ac && !_solvedMap.size())
		{
			tmp = this->parse(av[++i]);
			_solvedMap.resize(tmp->size() * tmp->size(), glm::ivec2(0, 0));
			tmp->getIvec2Map(_solvedMap);
		}
	}
	if (!_board && !_generate)
		throw std::logic_error("Error: No map to solve");
	return (1);
}

class Board	*Npuzzle::parse(char *path)
{
	size_t	size = 0;
	std::ifstream fileStream(path, std::ios::in);
	std::string lineMap("");
	std::stringstream mapstr;
	if(fileStream.is_open()){
		std::string line = "";
		int count = 0;
		while(getline(fileStream, line))
		{
			std::size_t found = line.find("#");
			if (found > 0)
			{
				std::string str = line.substr(0, found);
				std::string item;
				std::stringstream sstr(str);
				if (!size)
				{
					sstr >> size;
					if (size < 3)
						throw std::logic_error("Error: Invalid size !");
				}
				else
				{
					for(int x = 0; x < size; x++)
					{
						int nb;
						sstr >> nb;
						if(sstr.fail())
							throw std::logic_error("Error: Invalid map !");
						mapstr << nb;
						if (count != size * size)
							mapstr << " ";
						count++;
					}
				}
			}
		}
		fileStream.close();
		if (count != size * size)
			throw std::logic_error("Error: Map no valid!");
		std::cout << "Parsed Map: size " << size << std::endl << mapstr.str() << std::endl << std::endl;
		return (new Board(size, mapstr.str().c_str()));
	}
	else
		throw std::logic_error("Error: Impossible to open map");
	return (new Board(3, "6 3 5 2 1 7 0 8 4"));
}

class Board *Npuzzle::generate(void)
{
	class Board *board = NULL;
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
	board = new Board(_generateSize, map);
	std::cout << "Generated Map: size " << board->size() << std::endl << board->getHash() << std::endl << std::endl;
	return (board);
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
