#ifndef ASTAR_H
# define ASTAR_H

# include <queue>
# include <map>
# include <string>
# include <stack>
# include "node.h"
# include "struct.h"

# define MANHATTAN 0
# define DIJKSTRA 1

class Board;

class NodeCompare
{
public:
	bool operator() (const class Node *n1, const class Node *n2)
	{
		if (n1->getHeuristic() > n2->getHeuristic()
		|| (n1->getHeuristic() == n2->getHeuristic() && n1->getCost() >= n2->getCost()))
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
	int solve(void);

private:
	void searchChildren(class Node *node, t_node_prio_queue &children);
	class Node *getIfExist(std::map<std::string, class Node*> &map, std::string &key);
	void restorePath(std::stack<class Node*> &path);
	bool checkMoveValidity(class Board *b1, class Board *b2);
	int	countHeuristic(class Board *board);
	int manhattan(class Board *board);
	int dijkstra(class Board *board);

	t_node_prio_queue					_opened;
	std::map<std::string, class Node*>	_openedMap;
	std::map<std::string, class Node*>	_closed;
	std::vector<glm::ivec2>				&_solvedMap;
	unsigned long						_timeComplexity;
	unsigned long						_sizeComplexity;
	int									_heuristicUsed;
};

#endif
