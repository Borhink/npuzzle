/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npuzzle.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/02 20:43:52 by qhonore           #+#    #+#             */
/*   Updated: 2017/12/06 20:21:52 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "npuzzle.h"

Npuzzle::Npuzzle(char *map)
{
	(void)map;
	//PARSE MAP EASY PEASY
	_board = new class Board(3);
	_solvedMap.resize(_board->size() * _board->size(), {0, 0});
	_board->getSolvedPoints(_solvedMap);
	_board->printSolvedMap(_solvedMap);
}

Npuzzle::~Npuzzle()
{
	delete _board;
	_solvedMap.clear();
}
