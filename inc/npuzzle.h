#ifndef NPUZZLE_H
# define NPUZZLE_H

# include <thread>
# include <vector>
# include "struct.h"
# include "board.h"
# include "astar.h"

# define VALID 2
# define INVALID 3

class Npuzzle
{
public:
	Npuzzle(int ac, char **av);
	~Npuzzle();
	void							resolve(void);
	inline Board					*getBoard(void) { return (_board); }
	inline std::vector<glm::ivec2>	&getSolvedMap(void) { return (_solvedMap); }
	inline class Astar				*getAStar(void) { return (_astar); }
	inline bool						getDisplayGL(void) { return (_displayGL); }
	inline float					getAnimInMS(void) { return (_animInMS); }
private:
	int								countInversions(int i, int j, std::vector<std::vector<int>> &map);
	bool							checkIfSolvable(void);
	int								parseArgs(int ac, char **av);
	class Board						*parse(char *path);
	class Board						*generate(void);
	void							generateVectorMap(std::vector<std::vector<int>> &map, int len, int x, int y, int dir, int nb, int swap);

	std::thread						*_thread;
	class Board						*_board;
	std::vector<glm::ivec2>			_solvedMap;
	class Astar						*_astar;
	char							*_pathMap;
	bool							_generate;
	int								_generateSize;
	int								_generateParam;
	int								_heuristicParam;
	bool							_verboseParam;
	bool							_checkParam;
	bool							_displayGL;
	float							_animInMS;

};

#endif
