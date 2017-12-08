#include <iostream>
#include "board.h"

// 		0	1	2	3	4
//
// 0	1	2	3	4
//
// 1	12	11	14	5
//
// 2	11	0	15	6
//
// 3	10	9	8	7
//
// 4

Board::Board(size_t size, std::string hash) :
_size(size),
_hash(hash)
{
	_map.resize(_size);
	for (size_t i = 0; i < _map.size(); i++)
		_map[i].resize(_size, 0);
	this->parseHash(hash);
}

Board::Board(size_t size, std::vector<std::vector<int>> &map) :
_size(size),
_map(map)
{
	this->generateHash(map);
}

Board::~Board()
{
}

int Board::size() const
{
	return (_size);
}

void Board::printMap(void) const
{
	for (size_t y = 0; y < _map.size(); y++)
	{
		for (size_t x = 0; x < _map.size(); x++)
			std::cout << _map[y][x] << ' ';
		std::cout << std::endl;
	}
}

void Board::getSolvedPoints(std::vector<glm::ivec2> &map)
{
	this->generateSolvedPoints(map, _size, 0, 0, RIGHT, 1, 1);
}

////////////////////////////////////////////////////////////////////////////////
//								PRIVATES FUNCTIONS							  //
////////////////////////////////////////////////////////////////////////////////

void Board::generateSolvedPoints(std::vector<glm::ivec2> &map, int len, int x, int y, int dir, int nb, int swap)
{
	int	i = 0;

	if (len == 0)
		return;
	while (i < len)
	{
		map[nb % (_size * _size)] = {x, y};
		dir >= LEFT ? dir >= RIGHT ? dir >= UP ? y-- : x++ : x-- : y++;
		nb++;
		i++;
	}
	swap = (swap + 1) % 2;
	if (swap == 0) len--;
	if (dir == RIGHT)
		this->generateSolvedPoints(map, len, x - 1, y + 1, DOWN, nb, swap);
	if (dir == DOWN)
		this->generateSolvedPoints(map, len, x - 1, y - 1, LEFT, nb, swap);
	if (dir == LEFT)
		this->generateSolvedPoints(map, len, x + 1, y - 1, UP, nb, swap);
	if (dir == UP)
		this->generateSolvedPoints(map, len, x + 1, y + 1, RIGHT, nb, swap);

}

void Board::parseHash(std::string hash)
{
	for (size_t y = 0; y < _map.size(); y++)
	{
		for (size_t x = 0; x < _map.size(); x++)
		{
			_map[y][x] = std::stoi(hash);
			hash = hash.substr(hash.find(" ") + 1);
		}
	}
}

void Board::generateHash(std::vector<std::vector<int>> map)
{
	for (size_t y = 0; y < map.size(); y++)
	{
		for (size_t x = 0; x < map.size(); x++)
		{
			_hash += std::to_string(map[y][x]);
			if (y != map.size() - 1 || x != map.size() - 1)
				_hash += ' ';
		}
	}
}
