#ifndef PATTERNDATABASE_H
# define PATTERNDATABASE_H

#include <vector>
#include "board.h"

class PatternDatabase
{
public:
	PatternDatabase(class Board *board, std::vector<glm::ivec2> &solvedMap);
	~PatternDatabase();
	int manhattan(class Board *board);

private:
	void updatePattern();

	int						_size;
	long					_patternDist;
	std::vector<glm::ivec2>	&_solvedMap;
};

#endif
