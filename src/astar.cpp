#include <iostream>
#include "astar.h"
#include "board.h"

Astar::Astar(std::vector<glm::ivec2> &solvedMap, class Board *board) :
_solvedMap(solvedMap)
{
	opened.push(new class Node(5, board));
	opened.push(new class Node(4, board));
	opened.push(new class Node(4, board));
	opened.push(new class Node(7, board));
	while(!opened.empty())
	{
		std::cout << opened.top()->getCost() << std::endl;
		opened.pop();
	}
	std::cout << "Manhattan distance: " << this->manhattan(opened.top()->getBoard()) << std::endl;
}

Astar::~Astar()
{

}

int		Astar::manhattan(class Board *board)
{
	int dist = 0;
	std::vector<std::vector<int>> map = board->getMap();

	for (int y = 0; y < board->size(); y++)
	{
		for (int x = 0; x < board->size(); x++)
		{
			int nb = map[y][x];
			dist += abs(x - _solvedMap[nb][X]) + abs(y - _solvedMap[nb][Y]);
		}
	}
	return (dist);
}
