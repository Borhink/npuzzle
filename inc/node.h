#ifndef NODE_H
# define NODE_H

# include <string>

class Board;

class Node
{
public:
	Node(int cost, class Board *board);
	~Node();
	int getCost(void) const;
	inline Board *getBoard(void) { return (_board); }

private:
	class Board	*_board;
	int			_cost;
	// int			_heuristic;
	// int			_magic;
};

#endif
