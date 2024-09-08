/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Func_Utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 04:07:18 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/08 05:07:12 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Func_Utils.hpp"

std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t\n\r"); // Find the first non-whitespace character
	size_t last = str.find_last_not_of(" \t\n\r");   // Find the last non-whitespace character
	if (first == std::string::npos) // No non-whitespace characters found
		return "";
	return str.substr(first, last - first + 1);
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


bool	endOfCommand(const	std::string& str)
{
	int	i;

	i = -1;
	while (++i < str.length())
	{
		if (str[i] == '\n' || str[i] == '\r')
			return (1);
	}
	return (0);
}


std::vector<std::string> splitByCR(const std::string& data)
{
	std::vector<std::string> lines;
	std::istringstream ss(data);
	std::string line;
	// Read each line using getline with '\n' delimiter
	while (std::getline(ss, line, '\n')) {
		// Split each line based on '\r' character
		std::istringstream lineStream(line);
		std::string segment;
		while (std::getline(lineStream, segment, '\r'))
		{
			if (!segment.empty())
				lines.push_back(segment);
		}
	}
	return lines;
}



std::string converUpper(std::string str)
{
	int i;

	i = -1;
	while(str[++i])
		str[i] = toupper(str[i]);
	return str;
}

int checkCommand(std::string& comande)
{
	std::string command;

 	command = converUpper(comande);
	if (command == "PASS")
		return (1);
	else if(command == "NICK")
		return (2);
	else if(command == "USER")
		return (3);
	else if(command == "PRIVMSG")
		return (4);
	else if(command == "JOIN")
		return (5);
	else if(command == "TOPIC")
		return (6);
	else if(command == "KICK")
		return (7);
	else if(command == "INVITE")
		return (8);
	else if(command == "MODE")
		return (9);
	else if(command == "PART")
		return (10);
	return (0);
}
