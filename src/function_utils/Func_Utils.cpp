/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Func_Utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 04:07:18 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/09 08:30:58 by ymaaloum         ###   ########.fr       */
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
		unsigned int	i;

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

	int	checkCommand(std::string& comande)
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


	std::vector<std::string> splitBySpaces(const std::string &str)
	{
		std::vector<std::string> result;
		std::istringstream iss(str);
		std::string word;
		while (iss >> word)
			result.push_back(word);
		return result;
	}

	std::vector<std::string> split(const std::string& str, char delimiter)
	{
		std::vector<std::string> 	splitedString;
		std::string					s;
		std::stringstream			bs(str);
		while (std::getline(bs, s, delimiter))
			splitedString.push_back(s);
		return splitedString;
	}



	int		count_words(const std::string& str, bool &tr)
	{
		unsigned int	i;
		bool newword;
		unsigned int count;

		i = 0;
		newword = 1;
		count = 0;
		while (++i < str.size() && str[i] != ':')
		{
			if(str[i] != ' ' && newword == 1)
			{
				count++;
				newword = 0;
			}
			else if(str[i] == ' ' && newword == 0)
				newword = 1;
		}
		while (i < str.size())
		{
			if (str[i] != ':')
			{
				tr = 1;
				count++;
				break;
			}
			i++;
		}
		return (count);
	}

	void printf_message(const std::string& msg, int fd)
	{
		std::string response = msg;
		int sendResult = send(fd, response.c_str(), response.length(), 0);
		if (sendResult < 0)
			std::cerr << "Error sending response to client: " << strerror(errno) << std::endl;
		else if (sendResult == 0)
			std::cerr << "Client disconnected unexpectedly" << std::endl;
	}


int	checkChannelName(const std::string& name)
{
	if(name[0] == '#' || name[0] == '&')
		return (1);
	return(0);
}
