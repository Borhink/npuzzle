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
	int	manhattan(class Board *board);

private:
	t_node_prio_queue					opened;
	std::map<std::string, class Node*>	closed;
	std::vector<glm::ivec2>			&_solvedMap;
};

#endif
