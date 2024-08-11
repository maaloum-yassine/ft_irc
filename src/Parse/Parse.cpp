/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 04:09:12 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/08/10 23:36:27 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Parse.hpp"

Parse ::Parse(){};

void	Parse :: ParsePort(const std :: string& port)
{

	if (port.empty() || !CheckDigit(port)
		|| (std::atoi(port.c_str()) > 65535))
		throw std :: string ("ERREUR FOR PORT !");
}
Parse :: ~Parse()
{
}

