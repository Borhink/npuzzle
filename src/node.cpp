#include "node.h"

Node::Node(int cost, class Board *board):
_board(board),
_cost(cost)
{

}

Node::~Node()
{
}

int Node::getCost(void) const
{
	return (_cost);
}
