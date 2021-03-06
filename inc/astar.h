#ifndef ASTAR_H
# define ASTAR_H

# include <queue>
# include <map>
# include <string>
# include <stack>
# include <sstream>
# include "node.h"
# include "struct.h"


# define MANHATTAN (1 << 0)
# define LINEAR_CONFLICT (1 << 1)
# define OUT_ROW_COLUMN (1 << 2)
# define EUCLIDEAN (1 << 3)
# define MISPLACED (1 << 4)
# define DIJKSTRA (1 << 5)
# define HEURISTIC_COUNT (1 << 6)

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define EOC "\033[0m"

class Board;

class NodeCompare
{
public:
	bool operator() (const class Node *n1, const class Node *n2)
	{
		if (n1->getHeuristic() > n2->getHeuristic()
		|| (n1->getHeuristic() == n2->getHeuristic() && n1->getCost() > n2->getCost()))
			return (true);
		return (false);
	}
};

typedef std::priority_queue<class Node*, std::vector<class Node*>, NodeCompare> t_node_prio_queue;

class Astar
{
public:
	Astar(std::vector<glm::ivec2> &solvedMap, class Board *board,\
		int heuristicUsed, bool verbose, bool check);
	~Astar();
	int solve(void);
	inline bool		isSolved(void) { return (_solved); }
	void restorePath(std::stack<class Node*> &path);

private:
	void searchChildren(class Node *node, t_node_prio_queue &children);
	class Node *getIfExist(std::map<std::string, class Node*> &map, std::string &key);
	bool checkMoveValidity(class Board *b1, class Board *b2);
	int	countHeuristic(class Board *board);
	int manhattan(class Board *board);
	int linearConflict(class Board *board);
	int outOfRowOrColumn(class Board *board);
	int dijkstra(class Board *board);
	int euclidean(class Board *board);
	int misplaced(class Board *board);

	t_node_prio_queue					_opened;
	std::map<std::string, class Node*>	_openedMap;
	std::map<std::string, class Node*>	_closed;
	std::vector<glm::ivec2>				&_solvedMap;
	unsigned long						_timeComplexity;
	unsigned long						_sizeComplexity;
	int									_heuristicUsed;
	bool								_solved;
	bool								_verbose;
	bool								_check;
	std::clock_t						_time;
};

#endif
