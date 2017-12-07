/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npuzzle.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/02 20:38:52 by qhonore           #+#    #+#             */
/*   Updated: 2017/12/07 11:42:27 by mgallo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
