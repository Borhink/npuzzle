#ifndef BOARD_H
# define BOARD_H

# include <vector>
# include <string>
# include "struct.h"

# define	DOWN	2
# define	LEFT	4
# define	RIGHT	6
# define	UP		8

class Board
{
public:
	Board(size_t size, std::string hash);
	~Board();
	int								size() const;
	void							printMap(void) const;
	void							getSolvedPoints(std::vector<glm::ivec2> &map);
	std::vector<std::vector<int>>	&getMap(void) { return (_map); }
private:
	void generateSolvedPoints(std::vector<glm::ivec2> &map, int len, int x, int y, int dir, int nb, int swap);
	void parseHash(std::string hash);

	int								_size;
	std::vector<std::vector<int>>	_map;
	std::string						_hash;
};

#endif
