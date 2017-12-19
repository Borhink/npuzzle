#ifndef NODE_H
# define NODE_H

# include <string>
# include "board.h"

class Node
{
public:
	Node(int cost, int heuristic, class Board *board);
	~Node();
	inline int getCost(void) const { return (_cost); }
	inline int getHeuristic(void) const { return (_heuristic); }
	inline int getSum(void) const { return (_cost + _heuristic); }
	inline Board *getBoard(void) { return (_board); }
	inline std::string &getHash(void) { return (_board->getHash()); }
	inline std::vector<std::vector<int>> &getMap(void) { return (_board->getMap()); }

private:
	class Board	*_board;
	int			_cost;
	int			_heuristic;
};

#endif
