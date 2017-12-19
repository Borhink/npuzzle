#ifndef ASTAR_H
# define ASTAR_H

# include <queue>
# include <map>
# include <string>
# include "node.h"
# include "struct.h"

class Board;

class NodeCompare
{
public:
	bool operator() (const class Node *n1, const class Node *n2)
	{
		if (n1->getCost() > n2->getCost())
			return (true);
		return (false);
	}
};

typedef std::priority_queue<class Node*, std::vector<class Node*>, NodeCompare> t_node_prio_queue;

class Astar
{
public:
	Astar(std::vector<glm::ivec2> &solvedMap, class Board *board);
	~Astar();
	int manhattan(class Board *board);
	int solve(void);

private:
	void searchNeighbors(class Node *node, t_node_prio_queue &neighbors);

	t_node_prio_queue					_opened;
	std::map<std::string, class Node*>	_openedMap;
	std::map<std::string, class Node*>	_closed;
	std::vector<glm::ivec2>				&_solvedMap;
};

#endif
