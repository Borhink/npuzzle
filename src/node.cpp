#include "node.h"
#include "board.h"
#include <iostream>

Node::Node(int cost, class Board *board):
_board(board),
_cost(cost)
{

}

Node::Node(class Node *node, glm::ivec2 a, glm::ivec2 b):
_cost(node->getCost() + 1)
{
	std::vector<std::vector<int>> map = node->getBoard()->getMap();

	std::swap(map[a[Y]][a[X]], map[b[Y]][b[X]]);
	_board = new class Board(node->getBoard()->size(), map);
}

Node::~Node()
{
}

int Node::getCost(void) const
{
	return (_cost);
}
