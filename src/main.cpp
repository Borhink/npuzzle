/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 14:56:40 by qhonore           #+#    #+#             */
/*   Updated: 2017/12/06 19:58:59 by qhonore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "npuzzle.h"

int		main(int ac, char** av)
{
	if (ac < 2)
		std::cout << "Usage: ./npuzzle [map file]" << std::endl;
	else
	{
		class Npuzzle	npuzzle(av[1]);
	}
	return (0);
}
