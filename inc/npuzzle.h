#ifndef NPUZZLE_H
# define NPUZZLE_H

# include <vector>
# include "struct.h"
# include "board.h"

class Npuzzle
{
public:
	Npuzzle(char *map);
	~Npuzzle();

private:
	class Board				*_board;
	std::vector<glm::ivec2>	_solvedMap;
};

#endif
