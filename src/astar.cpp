#include <iostream>
#include "astar.h"
#include "board.h"

#include <unistd.h>

Astar::Astar(std::vector<glm::ivec2> &solvedMap, class Board *board,\
	int heuristicUsed, bool verbose, bool check) :
_solvedMap(solvedMap),
_timeComplexity(0),
_sizeComplexity(1),
_heuristicUsed(heuristicUsed),
_solved(false),
_verbose(verbose),
_check(check)
{
	class Node *start = new class Node(0, this->countHeuristic(board), board);

	_time = clock();
	_opened.push(start);
	_openedMap.emplace(start->getHash(), start);
	if (this->solve())
	{
		_solved = true;
		std::stack<class Node*> path;
		int i = 0, move;
		std::stringstream sstream;

		this->restorePath(path);
		_sizeComplexity = _opened.size() + _closed.size();
		move = path.size() - 1;
		while(!path.empty())
		{
			class Node *node = path.top();
			class Node *parent = node->getParent();
			class Board *board = node->getBoard();
			if (parent)
				sstream << node->getMove() << ", ";
			if (_verbose)
			{
				std::cout << "Cost: " << node->getCost();
				std::cout << ", Heuristic: " << node->getHeuristic() << std::endl;
				board->printMap();
				std::cout << "Movement : " << node->getMove() << std::endl;
			}
			if (_check)
			{
				if (!parent || (parent && this->checkMoveValidity(board, parent->getBoard())))
					std::cout << "MOVE " GREEN "OK" EOC << std::endl;
				else { std::cout << "MOVE " RED "KO" EOC << std::endl; break; }
				if (parent && node->getCost() - 1 != parent->getCost())
					{ std::cout << "MOVE " RED "KO" EOC << std::endl; break; }
			}
			if (_verbose)
				std::cout << "========================" << std::endl;
			i++;
			path.pop();
		}
		std::cout << "Time complexity: " << _timeComplexity << std::endl;
		std::cout << "Size complexity: " << _sizeComplexity << std::endl;
		_time = clock() - _time;
		std::cout << "Time : " << (float)_time / CLOCKS_PER_SEC << "s" << std::endl;
		std::cout << "Moves: " << move << std::endl;
		std::cout << "Movements :" << std::endl << sstream.str().substr(0, sstream.str().size() - 2) << std::endl;
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
		_timeComplexity++;
		node = _opened.top();
		board = node->getBoard();
		if (this->dijkstra(board) == 0)
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
				if (open)
					delete child;
				else
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
					children.push(new class Node(node->getCost() + 1, this->countHeuristic(newBoard), newBoard, node, GREEN "Left" EOC));
				}
				if (x < board->size() - 1)
				{
					std::swap(map[y][x], map[y][x + 1]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y][x + 1]);
					children.push(new class Node(node->getCost() + 1, this->countHeuristic(newBoard), newBoard, node, YELLOW "Right" EOC));
				}
				if (y > 0)
				{
					std::swap(map[y][x], map[y - 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y - 1][x]);
					children.push(new class Node(node->getCost() + 1, this->countHeuristic(newBoard), newBoard, node, MAGENTA "Up" EOC));
				}
				if (y < board->size() - 1)
				{
					std::swap(map[y][x], map[y + 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y + 1][x]);
					children.push(new class Node(node->getCost() + 1, this->countHeuristic(newBoard), newBoard, node, CYAN "Down" EOC));
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

bool Astar::checkMoveValidity(class Board *b1, class Board *b2)
{
	std::vector<std::vector<int>> &m1 = b1->getMap();
	std::vector<std::vector<int>> &m2 = b2->getMap();

	for (int y = 0; y < b1->size(); y++)
	{
		for (int x = 0; x < b1->size(); x++)
		{
			if (m1[y][x] != m2[y][x])
			{
				if (x < b1->size() - 1 && m1[y][x + 1] != m2[y][x + 1])
				{
					std::swap(m2[y][x], m2[y][x + 1]);
					class Board tmp(b1->size(), m2);
					if (b1->getHash().compare(tmp.getHash()) == 0)
					{
						std::swap(m2[y][x], m2[y][x + 1]);
						return (true);
					}
					else
					{
						std::swap(m2[y][x], m2[y][x + 1]);
						return (false);
					}

				}
				if (y < b1->size() - 1 && m1[y + 1][x] != m2[y + 1][x])
				{
					std::swap(m2[y][x], m2[y + 1][x]);
					class Board tmp(b1->size(), m2);
					if (b1->getHash().compare(tmp.getHash()) == 0)
					{
						std::swap(m2[y][x], m2[y + 1][x]);
						return (true);
					}
					else
					{
						std::swap(m2[y][x], m2[y + 1][x]);
						return (false);
					}
				}
			}
		}
	}
	return (false);
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

int	Astar::countHeuristic(class Board *board)
{
	int count = 0;

	if (_heuristicUsed & DIJKSTRA)
		return (this->dijkstra(board));
	if (_heuristicUsed & MANHATTAN)
		count += this->manhattan(board);
	if (_heuristicUsed & LINEAR_CONFLICT)
		count += this->linearConflict(board);
	if (_heuristicUsed & OUT_ROW_COLUMN)
		count += this->outOfRowOrColumn(board);
	if (_heuristicUsed & EUCLIDEAN)
		count += this->euclidean(board);
	if (_heuristicUsed & MISPLACED)
		count += this->misplaced(board);
	return (count);
}

int Astar::manhattan(class Board *board)
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

int Astar::linearConflict(class Board *board)
{
	int dist = 0;
	std::vector<std::vector<int>> map = board->getMap();

	for (int y = 0; y < board->size(); y++)
	{
		for (int x = 0; x < board->size(); x++)
		{
			int nb = map[y][x];
			if (nb && x < board->size() - 1 && y == _solvedMap[nb][Y])
			{
				for (int i = x + 1; i < board->size(); i++)
				{
					int nb2 = map[y][i];
					if (nb2 && y == _solvedMap[nb2][Y] && _solvedMap[nb2][X] < _solvedMap[nb][X])
						dist += 2;
				}
			}
		}
	}
	for (int x = 0; x < board->size(); x++)
	{
		for (int y = 0; y < board->size(); y++)
		{
			int nb = map[y][x];
			if (nb && y < board->size() - 1 && x == _solvedMap[nb][X])
			{
				for (int j = y + 1; j < board->size(); j++)
				{
					int nb2 = map[j][x];
					if (nb2 && x == _solvedMap[nb2][X] && _solvedMap[nb2][Y] < _solvedMap[nb][Y])
						dist += 2;
				}
			}
		}
	}
	return (dist);
}

int Astar::outOfRowOrColumn(class Board *board)
{
	int dist = 0;
	std::vector<std::vector<int>> map = board->getMap();

	for (int y = 0; y < board->size(); y++)
	{
		for (int x = 0; x < board->size(); x++)
		{
			int nb = map[y][x];
			if (nb)
			{
				if (_solvedMap[nb][X] != x)
					dist++;
				if (_solvedMap[nb][Y] != y)
					dist++;
			}
		}
	}
	return (dist);
}

int Astar::dijkstra(class Board *board)
{
	std::vector<std::vector<int>> map = board->getMap();

	for (int y = 0; y < board->size(); y++)
	{
		for (int x = 0; x < board->size(); x++)
		{
			int nb = map[y][x];
			if (x - _solvedMap[nb][X] != 0 || y - _solvedMap[nb][Y] != 0)
				return (1);
		}
	}
	return (0);
}

int Astar::euclidean(class Board *board)
{
	int dist = 0;
	std::vector<std::vector<int>> map = board->getMap();

	for (int y = 0; y < board->size(); y++)
	{
		for (int x = 0; x < board->size(); x++)
		{
			int nb = map[y][x];
			if (nb)
			{
				int xDist = x - _solvedMap[nb][X];
				int yDist = y - _solvedMap[nb][Y];
				dist += sqrt(pow(xDist , 2) + pow(yDist, 2));
			}
		}
	}
	return (dist);
}

int Astar::misplaced(class Board *board)
{
	int dist = 0;
	std::vector<std::vector<int>> map = board->getMap();

	for (int y = 0; y < board->size(); y++)
	{
		for (int x = 0; x < board->size(); x++)
		{
			int nb = map[y][x];
			if (nb && (x - _solvedMap[nb][X] != 0 || y - _solvedMap[nb][Y] != 0))
				dist++;
		}
	}
	return (dist);
}
