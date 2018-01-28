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
	Board(size_t size, std::vector<std::vector<int>> &map);
	~Board();
	int										size() const;
	void									printMap(void) const;
	void 									getIvec2Map(std::vector<glm::ivec2> &vec);
	void									getSolvedPoints(std::vector<glm::ivec2> &map);
	inline std::vector<std::vector<int>>	&getMap(void) { return (_map); }
	inline std::string						&getHash(void) { return (_hash); }
private:
	void generateSolvedPoints(std::vector<glm::ivec2> &map, int len, int x, int y, int dir, int nb, int swap);
	void parseHash(std::string hash);
	void generateHash(std::vector<std::vector<int>> map);

	int								_size;
	std::vector<std::vector<int>>	_map;
	std::string						_hash;
};

#endif
