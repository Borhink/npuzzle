#ifndef NPUZZLE_H
# define NPUZZLE_H

# include <thread>
# include <vector>
# include "struct.h"
# include "board.h"

class Npuzzle
{
public:
	Npuzzle(char *map);
	~Npuzzle();
	void							resolve(void);
	inline Board					*getBoard(void) { return (_board); }
	inline std::vector<glm::ivec2>	&getSolvedMap(void) { return (_solvedMap); }

private:
	Board							*parse(char *path);
	std::thread						*_thread;
	class Board						*_board;
	std::vector<glm::ivec2>			_solvedMap;

};

#endif
