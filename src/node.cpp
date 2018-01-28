#include "node.h"
#include "board.h"
#include <iostream>

Node::Node(int cost, int heuristic, class Board *board):
_parent(NULL),
_board(board),
_cost(cost),
_heuristic(heuristic),
_move("")
{
}

Node::Node(int cost, int heuristic, class Board *board, class Node *parent, std::string move):
_parent(parent),
_board(board),
_cost(cost),
_heuristic(heuristic),
_move(move)
{
}

Node::~Node()
{
	delete _board;
}
