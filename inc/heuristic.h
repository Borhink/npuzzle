/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heuristic.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 17:30:10 by qhonore           #+#    #+#             */
/*   Updated: 2017/12/07 12:08:31 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
