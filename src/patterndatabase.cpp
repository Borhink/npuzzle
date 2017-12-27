#include "patterndatabase.h"

PatternDatabase::PatternDatabase(class Board *board, std::vector<glm::ivec2> &solvedMap):
_size(board->size()),
_solvedMap(solvedMap)
{
	if (_size == 3)
		_patternDist = 0;
	else
		_patternDist = pow(_size, 3);
}

PatternDatabase::~PatternDatabase()
{
}

void PatternDatabase::updatePattern()
{
	if (_size > 3)
	{
		_size--;
		if (_size == 3)
			_patternDist = 0;
		else
			_patternDist = pow(_size, 3);
	}
}

int PatternDatabase::manhattan(class Board *board)
{
	int dist = 0;
	bool zero = false;
	std::vector<std::vector<int>> map = board->getMap();

	for (int y = 0; y < _size; y++)
	{
		for (int x = 0; x < _size; x++)
		{
			int nb = map[y][x];
			// if (nb == 0)
			// 	zero = true;
			if ((_solvedMap[nb][X] == _size - 1 || _solvedMap[nb][Y] == _size - 1 || _size == 3))
				dist += abs(x - _solvedMap[nb][X]) + abs(y - _solvedMap[nb][Y]);
		}
	}
	// if (!zero)
	// 	dist += pow(_size + 1, 3);
	if (dist == 0)
		this->updatePattern();
	return (dist + _patternDist);
}

// int PatternDatabase::manhattan(class Board *board)
// {
// 	int dist = 0;
// 	bool zero = false;
// 	std::vector<std::vector<int>> map = board->getMap();
//
// 	for (int y = 0; y < _size; y++)
// 	{
// 		for (int x = 0; x < _size; x++)
// 		{
// 			int nb = map[y][x];
// 			// if (nb == 0)
// 			// 	zero = true;
// 			if ((_solvedMap[nb][X] == _size - 1 || _solvedMap[nb][Y] == _size - 1 || _size == 3))
// 				dist += abs(x - _solvedMap[nb][X]) + abs(y - _solvedMap[nb][Y]);
// 		}
// 	}
// 	// if (!zero)
// 	// 	dist += pow(_size + 1, 3);
// 	if (dist == 0)
// 		this->updatePattern();
// 	return (dist + _patternDist);
// }


/*
1	2	3	4
12	13	14	5
11	0	15	6
10	9	8	7

MANHATTAN
Time complexity: 146420
Size complexity: 385357
Moves: 596



PATTERN
Time complexity: 166296
Size complexity: 292183
Moves: 110
NO
Time complexity: 1135
Size complexity: 2344
Moves: 102
*/
