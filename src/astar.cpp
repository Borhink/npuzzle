#include <iostream>
#include "astar.h"
#include "board.h"

#include <unistd.h>

Astar::Astar(std::vector<glm::ivec2> &solvedMap, class Board *board) :
_solvedMap(solvedMap),
_timeComplexity(0),
_sizeComplexity(1),
_heuristicUsed(MANHATTAN),
_patternDatabase(board, solvedMap),
_solved(false)
{
	class Node *start = new class Node(0, this->countHeuristic(board), board);

	_opened.push(start);
	_openedMap.emplace(start->getHash(), start);
	if (this->solve())
	{
		_solved = true;
		std::stack<class Node*> path;
		int i = 0, move;

		this->restorePath(path);
		_sizeComplexity = _opened.size() + _closed.size();
		move = path.size() - 1;
		while(!path.empty())
		{
			class Node *node = path.top();
			class Node *parent = node->getParent();
			class Board *board = node->getBoard();
			std::cout << "Cost: " << node->getCost();
			std::cout << ", Heuristic: " << node->getHeuristic() << std::endl;
			board->printMap();
			//////////////////////////////DEBUG//////////////////////////////////////////////
			if (!parent || (parent && this->checkMoveValidity(board, parent->getBoard())))
				std::cout << "MOVE OK" << std::endl;
			else { std::cout << "WRONG MOVE" << std::endl; break; }
			std::cout << "========================" << std::endl;
			//////////////////////////////////FIN DEBUG//////////////////////////////////////
			i++;
			path.pop();
		}
		std::cout << "Time complexity: " << _timeComplexity << std::endl;
		std::cout << "Size complexity: " << _sizeComplexity << std::endl;
		std::cout << "Moves: " << move << std::endl;
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
int i = 0;
	while (!_opened.empty())
	{
		_timeComplexity++;
		node = _opened.top();
		board = node->getBoard();
		//////////////////////////////DEBUG//////////////////////////////////////////////
		std::cout << "=========== " << i << " =============" << std::endl;
		std::cout << "Cost: " << node->getCost();
		std::cout << ", Heuristic: " << node->getHeuristic() << std::endl;
		board->printMap();
		i++;
		std::cout << "=============================" << std::endl;
		// usleep(500000);
		//////////////////////////////DEBUG//////////////////////////////////////////////
		if (this->dijkstra(board) == 0 || node->getHeuristic() == 0)
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
				{
					open->setParent(child->getParent());
					open->setCost(child->getCost());
					open->setHeuristic(child->getHeuristic());
					delete child;
				}
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
					children.push(new class Node(node->getCost() + 1, this->countHeuristic(newBoard), newBoard, node));
				}
				if (x < board->size() - 1)
				{
					std::swap(map[y][x], map[y][x + 1]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y][x + 1]);
					children.push(new class Node(node->getCost() + 1, this->countHeuristic(newBoard), newBoard, node));
				}
				if (y > 0)
				{
					std::swap(map[y][x], map[y - 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y - 1][x]);
					children.push(new class Node(node->getCost() + 1, this->countHeuristic(newBoard), newBoard, node));
				}
				if (y < board->size() - 1)
				{
					std::swap(map[y][x], map[y + 1][x]);
					newBoard = new class Board(board->size(), map);
					std::swap(map[y][x], map[y + 1][x]);
					children.push(new class Node(node->getCost() + 1, this->countHeuristic(newBoard), newBoard, node));
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
				if (x < b1->size() - 1 &&  m1[y][x + 1] != m2[y][x + 1])
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
				if (y < b1->size() - 1 &&  m1[y + 1][x] != m2[y + 1][x])
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
	if (_heuristicUsed & PATTERN_DATABASE)
		return (this->patternDatabase(board));
	if (_heuristicUsed & MANHATTAN)
		count += this->manhattan(board);
	if (_heuristicUsed & LINEAR_CONFLICT)
		count += this->linearConflict(board);
	if (_heuristicUsed & OUT_ROW_COLUMN)
		count += this->OutOfRowOrColumn(board);
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
	return (dist);
}

int Astar::OutOfRowOrColumn(class Board *board)
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


int Astar::patternDatabase(class Board *board)
{
	return (_patternDatabase.manhattan(board));
}

/*
========================================================================
MAP 4 :
3 14 4 5
2 12 0 15
13 1 11 6
10 9 8 7
==============================================================
DIJKSTRA
Time complexity: 2107367
Size complexity: 4071649
Moves: 20
./npuzzle 3_16.map  150.75s user 15.40s system 135% cpu 2:02.30 total
==============================================================
MANHATTAN
Time complexity: 156
Size complexity: 332
Moves: 30
./npuzzle 3_16.map  0.61s user 0.27s system 40% cpu 2.176 total
==============================================================
LINEAR_CONFLICT
Time complexity: 2297560
Size complexity: 4408809
Moves: 20
./npuzzle 3_16.map  153.33s user 0.26s system 44% cpu 2.085 total
==============================================================
OUT_ROW_COLUMN
Time complexity: 81
Size complexity: 181
Moves: 22
./npuzzle 3_16.map  0.70s user 0.28s system 46% cpu 2.108 total
==============================================================
LINEAR_CONFLICT + OUT_ROW_COLUMN
Time complexity: 125
Size complexity: 272
Moves: 30
./npuzzle 3_16.map  0.67s user 0.25s system 49% cpu 1.860 total
==============================================================
MANHATTAN + OUT_ROW_COLUMN
Time complexity: 115
Size complexity: 256
Moves: 36
./npuzzle 3_16.map  0.62s user 0.24s system 46% cpu 1.843 total
==============================================================
MANHATTAN + LINEAR_CONFLICT
Time complexity: 72
Size complexity: 153
Moves: 30
./npuzzle 3_16.map  0.51s user 0.19s system 44% cpu 1.576 total
==============================================================
MANHATTAN + LINEAR_CONFLICT + OUT_ROW_COLUMN
Time complexity: 82
Size complexity: 173
Moves: 30
./npuzzle 3_16.map  0.47s user 0.17s system 42% cpu 1.476 total
========================================================================
*/
