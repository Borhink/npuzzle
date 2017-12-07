#include "npuzzle.h"
#include <stdexcept>

Npuzzle::Npuzzle(char *map)
{
	(void)map;
	//PARSE MAP EASY PEASY
	_board = new class Board(3, "1 2 3 8 6 4 0 7 5");
	_solvedMap.resize(_board->size() * _board->size(), glm::ivec2(0, 0));
	_thread = new std::thread(&Npuzzle::resolve, this);
	_thread->detach();
}

Npuzzle::~Npuzzle()
{
	delete _thread;
	delete _board;
	_solvedMap.clear();
}

void	Npuzzle::resolve(void)
{
	_board->getSolvedPoints(_solvedMap);
	_board->printMap();
}
