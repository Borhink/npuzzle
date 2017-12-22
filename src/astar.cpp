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
			std::cout << "Cost: " << node->getCost();
			std::cout << ", Heuristic: " << node->getHeuristic() << std::endl;
			board->printMap();
			std::cout << "========================" << std::endl;
			i++;
			path.pop();
		}
		std::cout << "Number of states in opened: " << _opened.size() << std::endl;
		std::cout << "Number of states in closed: " << _closed.size() << std::endl;
		// std::cout << "Maximum states selected: " << _opened.size() + _closed.size() + _unsavedStates << std::endl;
		std::cout << "Maximum states stored: " << _opened.size() + _closed.size() << std::endl;
		std::cout << "Moves from initial state to final state: " << i - 1 << std::endl;
	}
}

// bool Astar::checkMoveValidity(class Board *b1, class Board *b2)
// {
// 	std::vector<std::vector<int>> &m1 = b1->getMap();
// 	std::vector<std::vector<int>> &m2 = b2->getMap();
// 	int moves = 0;
//
// 	for (int y = 0; y < b1->size(); y++)
// 	{
// 		for (int x = 0; x < b1->size(); x++)
// 		{
// 			if (m1[y][x] != m2[y][x])
// 			{
// 				if ()
// 			}
// 		}
// 	}
// 		return (false);
// 	return (true);
// }

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
}

int	Astar::solve(void)
{
	class Node *node, *child, *close, *open;
	class Board *board;
	t_node_prio_queue children;
	class NodeCompare compare;

	while (!_opened.empty())
	{
		node = _opened.top();
		board = node->getBoard();
		if (this->manhattan(board) == 0)
			return (1);
		_opened.pop();
		this->searchChildren(node, children);
		while (!children.empty())
		{
			child = children.top();
			close = this->getIfExist(_closed, child->getHash());
			open = this->getIfExist(_openedMap, child->getHash());
			if ((open && compare(child, open)) || (close && compare(child, close)))
				delete child;
			else
			{
				// if open -> open moins bon
					//JE DEVRAIS METTRE MON child QUI EST MEILLEUR, sauf que infinite loop
				if (!open)
				{
					_opened.push(child);
					_openedMap.emplace(child->getHash(), child);
				}
			}
			children.pop();
		}
		_closed.emplace(node->getHash(), node);
	}
	return (1);
}

void	Astar::searchChildren(class Node *node, t_node_prio_queue &children)
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
					children.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard, node));
				}
				if (x < board->size() - 1)
				{
					std::swap(map[y][x], map[y][x + 1]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y][x + 1]);
					children.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard, node));
				}
				if (y > 0)
				{
					std::swap(map[y][x], map[y - 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y - 1][x]);
					children.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard, node));
				}
				if (y < board->size() - 1)
				{
					std::swap(map[y][x], map[y + 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y + 1][x]);
					children.push(new class Node(node->getCost() + 1, this->manhattan(newBoard), newBoard, node));
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
			// if (nb)
				dist += abs(x - _solvedMap[nb][X]) + abs(y - _solvedMap[nb][Y]);
		}
	}
	return (dist);
}

/*
============   >= 				============
NORMAL
Number of states in opened: 1073
Number of states in closed: 801
Maximum states stored: 1874
Moves from initial state to final state: 94

VERY_HARD
Number of states in opened: 12224
Number of states in closed: 9653
Maximum states stored: 21877
Moves from initial state to final state: 158

HARDCORE
Number of states in opened: 4128
Number of states in closed: 3151
Maximum states stored: 7279
Moves from initial state to final state: 137
============================================

============   > 				============
NORMAL
Number of states in opened: 1030
Number of states in closed: 788
Maximum states stored: 1818
Moves from initial state to final state: 94

VERY_HARD


HARDCORE
Number of states in opened: 9844
Number of states in closed: 7453
Maximum states stored: 17297
Moves from initial state to final state: 169
============================================

============   >  ET	if (nb) ============
NORMAL 4
Number of states in opened: 1209
Number of states in closed: 1132
Maximum states stored: 2341
Moves from initial state to final state: 102

VERY_HARD 4
Number of states in opened: 2230
Number of states in closed: 1977
Maximum states stored: 4207
Moves from initial state to final state: 132

HARDCORE
Number of states in opened: 3018
Number of states in closed: 2939
Maximum states stored: 5957
Moves from initial state to final state: 129
============================================

============   >= ET	if (nb) ============
NORMAL
Number of states in opened: 1183
Number of states in closed: 1112
Maximum states stored: 2295
Moves from initial state to final state: 102

VERY_HARD
Number of states in opened: 2212
Number of states in closed: 2109
Maximum states stored: 4321
Moves from initial state to final state: 152

HARDCORE
Number of states in opened: 2954
Number of states in closed: 2811
Maximum states stored: 5765
Moves from initial state to final state: 139
============================================
*/
