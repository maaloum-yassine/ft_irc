/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 20:12:14 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/08/15 20:39:13 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/client.hpp"


client::client(int fd,const std::string ip,const std::string hostname):_fd(fd),_ip(ip),_hostname(hostname)
{

}


const	std::string&	client::get_nickname() const {
	return this->_nickname;
}

const	std::string&	client::get_username() const {
	return this->_username;
}

const	std::string&	client::get_realname() const {
	return this->_realname;
}

const	std::string&	client::get_hostname() const {
	return this->_hostname;
}

int		client::get_fd() const {
	return this->_fd;
}


void	client::set_realname(const std::string& RealName)
{
	this->_realname = RealName;
}

void	client::set_username(const  std::string& UserName)
{
	this->_username = UserName;
}

void	client:: set_nickname(const std::string& nick)
{
	this->_nickname = nick;
}

client :: ~client()
{

}
