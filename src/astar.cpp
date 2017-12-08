#include <iostream>
#include "astar.h"
#include "board.h"

Astar::Astar(std::vector<glm::ivec2> &solvedMap, class Board *board) :
_solvedMap(solvedMap)
{
	int heuristic = this->manhattan(board);
	opened.push(new class Node(0, heuristic, board));
	this->solve();
}

Astar::~Astar()
{
	while(!opened.empty())
	{
		delete opened.top();
		opened.pop();
	}
	for (auto it = closed.begin(); it != closed.end(); ++it)
		delete it->second;
	closed.clear();
}

int Astar::solve(void)
{
	class Node *node;
	class Board *board;
	t_node_prio_queue neighbors;

	while(!opened.empty())
	{
		node = opened.top();
		board = node->getBoard();
		this->searchNeighbors(node, neighbors);
		std::cout << "Manhattan distance: " << this->manhattan(board) << std::endl;
		if (this->manhattan(board) == 0)
			return (1);
		opened.pop();
	}
	return (0);
}

void	Astar::searchNeighbors(class Node *node, t_node_prio_queue &neighbors)
{
	class Board *board = node->getBoard();
	class Board *newBoard;
	std::vector<std::vector<int>> map = board->getMap();

	for (int y = 0; y < board->size(); y++)
	{
		for (int x = 0; x < board->size(); x++)
		{
			if (map[y][x] == 0)
			{
				if (x > 0)
				{
					std::swap(map[y][x], map[y][x - 1]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y][x - 1]);
					neighbors.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard));
				}
				if (x < board->size() - 1)
				{
					std::swap(map[y][x], map[y][x + 1]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y][x + 1]);
					neighbors.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard));
				}
				if (y > 0)
				{
					std::swap(map[y][x], map[y - 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y - 1][x]);
					neighbors.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard));
				}
				if (y < board->size() - 1)
				{
					std::swap(map[y][x], map[y + 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y + 1][x]);
					neighbors.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard));
				}
			}
		}
	}
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
