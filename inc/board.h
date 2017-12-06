/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/02 20:38:52 by qhonore           #+#    #+#             */
/*   Updated: 2017/12/06 20:09:30 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_H
# define BOARD_H

# include <vector>
# include "struct.h"

# define	DOWN	2
# define	LEFT	4
# define	RIGHT	6
# define	UP		8

class Board
{
public:
	Board(size_t size);
	~Board();
	int	size() const;
	void printSolvedMap(std::vector<t_point> map) const;
	void getSolvedPoints(std::vector<t_point> &map);

private:
	void generateSolvedPoints(std::vector<t_point> &map, int len, int x, int y, int dir, int nb, int swap);

	int						_size;
};

#endif
