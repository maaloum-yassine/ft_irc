/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 20:12:14 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/10 23:41:41 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/client.hpp"


client::client(int fd,const std::string& ip):_fd(fd),_ipclient(ip)
{
	_nickname = "";
	_username = "";
	_duplicateNickname = 0;
	_passB = 0;
	_nicknameB = 0;
	_usernameB = 0;
	_connected = 0;

}


void	client::addFd(int fd)
{
	this->_fd = fd;
	_passB = true;
}

void    client::addNickname(std::string const& nickname)
{

	if (_passB && !nickname.empty())
	{
		this->_nickname =  nickname;
		_nicknameB = 1;
		if (_usernameB == 1)
		{
			std :: cout << "||||||" << std :: endl;
			_connected = 1;
			std::cout << _username << std::endl;
		}
	}

}

void	client::addUser(std::string const & username)
{
	if (_passB && !username.empty())
	{
		this->_username = username;
		_usernameB = 1;
		if (_nicknameB == 1)
		{
			_connected = 1;
			std::cout << "this" << _nickname  << std::endl;
		}
	}
}

client :: ~client()
{

}
