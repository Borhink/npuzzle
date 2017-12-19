#include "node.h"
#include "board.h"
#include <iostream>

Node::Node(int cost, int heuristic, class Board *board):
_board(board),
_cost(cost),
_heuristic(heuristic)
{
	_board->printMap();
	std::cout << "_cost: " << _cost << std::endl;
	std::cout << "_heuristic: " << _heuristic << std::endl;
}

Node::~Node()
{
	delete _board;
}
