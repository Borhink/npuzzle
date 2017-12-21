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

	if (this->solve())
	{
		std::stack<class Node*> path;
		int i = 0;

		this->restorePath(path);
		while(!path.empty())
		{
			class Node *node = path.top();
			class Board *board = node->getBoard();
			std::cout << "====== Path " << i << " ======" << std::endl;
			board->printMap();
			std::cout << "Cost: " << node->getCost() << std::endl;
			std::cout << "Heuristic: " << node->getHeuristic() << std::endl;
			i++;
			path.pop();
		}
	}
}

void Astar::restorePath(std::stack<class Node*> &path)
{
	class Node *node = _opened.top();

	while (node)
	{
		path.push(node);
		node = node->getParent();
	}
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

int	Astar::solve(void)
{
	class Node *node, *open, *close;
	class Board *board;
	t_node_prio_queue neighbors;
	class Node *neighbor;

	while(!_opened.empty())
	{
		node = _opened.top();
		board = node->getBoard();
		if (this->manhattan(board) == 0)
			return (1);
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
					neighbors.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard, node));
				}
				if (x < board->size() - 1)
				{
					std::swap(map[y][x], map[y][x + 1]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y][x + 1]);
					neighbors.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard, node));
				}
				if (y > 0)
				{
					std::swap(map[y][x], map[y - 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y - 1][x]);
					neighbors.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard, node));
				}
				if (y < board->size() - 1)
				{
					std::swap(map[y][x], map[y + 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y + 1][x]);
					neighbors.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard, node));
				}
			}
		}
	}
}

class Node* Astar::getIfExist(std::map<std::string, class Node*> &map, std::string &key)
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
