/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:54:02 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/08/10 23:43:34 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"

int main (int ac , char **av)
{
	try
	{
		if (ac != 3)
			throw std::runtime_error("The prog take two arguments port password [^_^]");
		server Server_Irc((std :: string(av[1])),(std :: string(av[2])) );
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}
