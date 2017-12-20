#include <iostream>
#include "astar.h"
#include "board.h"

Astar::Astar(std::vector<glm::ivec2> &solvedMap, class Board *board) :
_solvedMap(solvedMap)
{
	int heuristic = this->manhattan(board);
	class Node *start = new class Node(0, heuristic, board);
	_opened.push(start);
	_openedMap.emplace(start->getHash(), start);
	this->solve();
}

Astar::~Astar()
{
	while(!_opened.empty())
	{
		delete _opened.top();
		_opened.pop();
	}
	for (auto it = _closed.begin(); it != _closed.end(); ++it)
		delete it->second;
	_closed.clear();
	_openedMap.clear();
}

int Astar::solve(void)
{
	class Node *node, *open, *close;
	class Board *board;
	t_node_prio_queue neighbors;
	class Node *neighbor;
	int i = 0;

	while(!_opened.empty())
	{
		node = _opened.top();
		board = node->getBoard();
		////////////// PRINT TEST ////////////////////////
		std::cout << "====== Move " << i << " ======" << std::endl;
		board->printMap();
		std::cout << "Cost: " << node->getCost() << std::endl;
		std::cout << "Heuristic: " << node->getHeuristic() << std::endl;
		i++;
		/////////////////////////////////////////////////
		if (this->manhattan(board) == 0)
		{
			std::cout << "=================== END ALGO ===================\n";
			return (1);
		}
		this->searchNeighbors(node, neighbors);
		while (!neighbors.empty())
		{
			neighbor = neighbors.top();
			open = this->getIfExist(_openedMap, neighbor->getHash());
			close = this->getIfExist(_closed, neighbor->getHash());
			if ((open && neighbor->getSum() > open->getSum()) || (close && neighbor->getSum() > close->getSum()))
				delete neighbor;
			else
			{
				if (open)
				{
					open->setCost(neighbor->getCost());
					open->setHeuristic(neighbor->getHeuristic());
					delete neighbor;
				}
				else
				{
					_opened.push(neighbor);
					_openedMap.emplace(neighbor->getHash(), neighbor);
				}
			}
			neighbors.pop();
		}
		_closed.emplace(node->getHash(), node);
		_opened.pop();
	}
	std::cout << "=================== NO END ===================\n";
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

Node* Astar::getIfExist(std::map<std::string, class Node*> &map, std::string &key)
{
	if (map.find(key) != map.end())
		return (map.at(key));
	return (NULL);
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
			if (nb)
				dist += abs(x - _solvedMap[nb][X]) + abs(y - _solvedMap[nb][Y]);
		}
	}
	return (dist);
}
