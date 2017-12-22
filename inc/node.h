#ifndef NODE_H
# define NODE_H

# include <string>
# include "board.h"

class Node
{
public:
	Node(int cost, int heuristic, class Board *board);
	Node(int cost, int heuristic, class Board *board, class Node *parent);
	~Node();
	inline class Node *getParent(void) { return (_parent); }
	inline void setParent(class Node *parent) { _parent = parent; }
	inline int getCost(void) const { return (_cost); }
	inline void setCost(int cost) { _cost = cost; }
	inline int getHeuristic(void) const { return (_heuristic); }
	inline void setHeuristic(int heuristic) { _heuristic = heuristic; }
	inline int getSum(void) const { return (_cost + _heuristic); }
	inline Board *getBoard(void) { return (_board); }
	inline std::string &getHash(void) { return (_board->getHash()); }
	inline std::vector<std::vector<int>> &getMap(void) { return (_board->getMap()); }

private:
	class Node	*_parent;
	class Board	*_board;
	int			_cost;
	int			_heuristic;
};

#endif
