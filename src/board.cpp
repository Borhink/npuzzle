/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/02 20:43:52 by qhonore           #+#    #+#             */
/*   Updated: 2017/12/07 11:54:31 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "board.h"

// 		0	1	2	3	4
//
// 0	1	2	3	4
//
// 1	12	11	14	5
//
// 2	11	0	15	6
//
// 3	10	9	8	7
//
// 4

Board::Board(size_t size) : _size(size)
{
}

Board::~Board()
{
}

int Board::size() const
{
	return (_size);
}

void Board::printSolvedMap(std::vector<glm::ivec2> map) const
{
	for (size_t i = 0; i < map.size(); i++)
	{
		std::cout << i << ": " << map[i][1] << ", " << map[i][0] << std::endl;
	}
}

void Board::getSolvedPoints(std::vector<glm::ivec2> &map)
{
	generateSolvedPoints(map, _size, 0, 0, RIGHT, 1, 1);
}

////////////////////////////////////////////////////////////////////////////////
//								PRIVATES FUNCTIONS							  //
////////////////////////////////////////////////////////////////////////////////

void Board::generateSolvedPoints(std::vector<glm::ivec2> &map, int len, int x, int y, int dir, int nb, int swap)
{
	int	i = 0;

	if (len == 0)
		return;
	while (i < len)
	{
		map[nb % (_size * _size)] = {x, y};
		dir >= LEFT ? dir >= RIGHT ? dir >= UP ? y-- : x++ : x-- : y++;
		nb++;
		i++;
	}
	swap = (swap + 1) % 2;
	if (swap == 0) len--;
	if (dir == RIGHT)
		this->generateSolvedPoints(map, len, x - 1, y + 1, DOWN, nb, swap);
	if (dir == DOWN)
		this->generateSolvedPoints(map, len, x - 1, y - 1, LEFT, nb, swap);
	if (dir == LEFT)
		this->generateSolvedPoints(map, len, x + 1, y - 1, UP, nb, swap);
	if (dir == UP)
		this->generateSolvedPoints(map, len, x + 1, y + 1, RIGHT, nb, swap);

}
