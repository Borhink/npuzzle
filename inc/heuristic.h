#ifndef HEURISTIC_H
# define HEURISTIC_H

# include "struct.h"

class Board;

class Heuristic
{
public:

	static int	manhattan(const class Board &board);
	// static int	misplacedTiles(t_point a, t_point b);
	// static int	patternDatabase(t_point a, t_point b);
};

#endif
