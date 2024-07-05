/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Func_Utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 04:07:18 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/07/05 05:17:53 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Func_Utils.hpp"

std :: string trim (const std :: string &str)
{
	int	start;
	int	len;

	start = -1;
	len = str.length();
	while (++start < len && std::isspace(str[start]))
	;
	++len ;
	while (--len > start && std::isspace(str[len - 1]))
	;
	return str.substr(start, len - start);
}


bool	CheckDigit(const std:: string &str)
{
	int	i;

	i = 0;
	if (str[0] == '+')
		++i;
	while (str.c_str()[i])
	{
		if (!std::isdigit(str[i++]))
			return (0);
	}
	return (1);
}
