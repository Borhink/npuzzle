#ifndef NODE_H
# define NODE_H

# include <string>
# include "struct.h"

class Board;

class Node
{
public:
	Node(int cost, class Board *board);
	Node(class Node *node, glm::ivec2 swap_a, glm::ivec2 swap_b);
	~Node();
	int getCost(void) const;
	inline Board *getBoard(void) { return (_board); }

private:
	class Board	*_board;
	int			_cost;
	// int			_heuristic;
};

#endif
