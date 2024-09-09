/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:37:50 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/09 22:03:20 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"

server::server(const std:: string& port, const std:: string& password):_port(port),_password(password),_serv_socket_fd(-1)
{
	try
	{
		this->_parse.ParsePort(trim(_port));
		this->start_server();
	}
	catch(const std::string& err)
	{
		if (_serv_socket_fd != -1)
			close(_serv_socket_fd);
		std :: cerr << err << std :: endl;
		exit (EXIT_FAILURE);
	}
}

	void server:: display_err(const std::string& err, bool exit)const
	{
		std::cerr << strerror(errno) << std::endl;
		if (exit)
			throw std :: string (err);
		else
			std::cerr << err << std::endl;
	}

	void	server :: create_socket()
	{
		struct sockaddr_in server_addr;
		int opt = 1;

		if ((_serv_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw std :: string ("ERROR CREATING SOCKET !!");
		if (setsockopt(_serv_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
			display_err("ERROR SET SOCKET !!", 1);
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(std::atoi(this->_port.c_str()));
		if (bind(_serv_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
			display_err("ERROR BIND !!", 1);
		if (fcntl(_serv_socket_fd, F_SETFL, O_NONBLOCK)) //cette ligne de code fait en sorte que ton serveur puisse gérer les connexions sans être bloqué sur une seule, rendant ton serveur plus rapide et efficace.
			display_err("ERROR FCNTL !!", 1);
		if (listen(_serv_socket_fd, SOMAXCONN))
			display_err("ERROR LISTEN !!", 1);
	}

	void	server :: handle_cnx_client()
	{
		struct sockaddr_in		client_addr;
		int						client_fd;
		socklen_t				client_len = sizeof(client_addr);

		if ((client_fd = accept(this->_serv_socket_fd, (struct sockaddr*)&client_addr, &client_len)) == -1)
		{
			display_err("ERROR ACCEPT !!", 0);
			return ;
		}
		if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
		{
			display_err("ERROR FCNTL !!", 0);
			return ;
		}
		struct pollfd		client_pollfd;
		client_pollfd.fd = client_fd;
		client_pollfd.events = POLLIN | POLLERR | POLLHUP;
		this->_fds.push_back(client_pollfd);
		std::string clientIp = inet_ntoa(client_addr.sin_addr);
		std::map<int, client*>::iterator it = this->_client.find(client_fd);
		if (it == this->_client.end())
			this->_client[client_fd] = new client(client_fd, clientIp);
		std::cout << "New connection from " << clientIp << "fd is -> "<< client_fd << std::endl;
	}

	void server :: 	start_server()
	{
		create_socket();
		struct pollfd server_pollfd;
		server_pollfd.fd = this->_serv_socket_fd;
		server_pollfd.events = POLLIN | POLLERR | POLLHUP; // events to monitor: POLLIN: there's data to read, POLLERR: there's an error, POLLHUP: the client disconnected
		this->_fds.push_back(server_pollfd);
		std::cout << "Server started on 0.0.0.0 : " << this->_port << std::endl;
		while (1)
		{
			if (poll(this->_fds.data(), this->_fds.size(), -1) < 0)
				display_err("ERROR POLL !!", 1);
			else if (this->_fds[0].revents & POLLIN)
				handle_cnx_client();
			for (size_t i = 1; i < _fds.size(); ++i)
			{
				if (_fds[i].revents & POLLIN)
				{
					this->handle_msg_client(i);
					break ;
				}
			}
		}
	}

	void	server :: handle_msg_client(size_t& index)
	{

		std :: string msg_env;
		char	buffer[4096];
		int		byt_rd;

		byt_rd = recv(_fds[index].fd, buffer, sizeof(buffer), 0);
		switch (byt_rd)
		{
			case -1:
				display_err(" ERROR RECEIVING DATA FROM CLIENT", 0);
				return ;
			case 0 :
				std::cout << "Client disconnected. Socket: " << this->_fds[index].fd << std::endl;
				close(this->_fds[index].fd);
				std::swap(this->_fds[index--], this->_fds.back());
				this->_fds.pop_back();
				return ;
			default:
				buffer[byt_rd] = '\0';
				msg_env = std::string(buffer, byt_rd);
				// std :: cout << "messgae is" << msg_env << std :: endl;
		}
		if (endOfCommand(msg_env))
		{
			std::vector<std::string> vt = splitByCR(msg_env);
			size_t j = 0;
			while(j < vt.size())
				std::cout << vt[j++] << std::endl;
			this->commandApply(this->_fds[index].fd, vt);
		}
	}


	/********************************************************************************/
	void	check_password(client _client ,const std::string & pass, const std::string& pass_serv, int fd)
	{
		if (pass == pass_serv)
		{
			printf_message("Correct password\n\r", fd);
			_client._passB = 1;
			std::cout << "passb" << _client._passB << std::endl;
			std::cout << "nicknameb" << _client._nicknameB<< std::endl;
			std::cout << "usernameB" << _client._usernameB<< std::endl;
			if(_client._passB && _client._nicknameB && _client._usernameB)
			{
				std::string msg = ":ircserv_KAI.chat 001 " + _client._nickname + " :Welcome to the IRC Network, " + _client._nickname + " \r\n";
				printf_message(msg, fd);
			}
		}
		else
			printf_message("Wrong password\n", fd);
	}
	/********************************************************************************/

	bool	server::alreadyUsedNickname(const std::string& nickname)
	{
		std::map<int, client*>::iterator it  = this->_client.begin();
		while(it != _client.end())
		{
			if(it->second->_nickname == nickname)
				return (1);
			++it;
		}
		return (0);
	}

	void	nick(client _client ,const std::string & _nicknm, int fd)
	{
		_client.addNickname(_nicknm);
		printf_message(RPL_NICK_SET(_client._ipclient, _nicknm), fd);
		_client._nicknameB = 1;
		if(_client._passB && _client._nicknameB && _client._usernameB)
			printf_message( ":ircserv_KAI.chat 001 " + _client._nickname + " :Welcome to the IRC Network, " + _client._nickname + " \r\n", fd);
	}
	/********************************************************************************/


	void	prive_msg(client _client, const std:: string& commandLine,int fd)
	{

		bool	tr;

		tr = 0;
		std::vector<std::string>username = split(commandLine, ':');
		if(count_words(commandLine, tr) == 5 && tr)
		{
			std::cout << username[1] << std::endl;
			_client.addUser(username[1]);
			_client._usernameB = 1;
			if(_client._passB && _client._nicknameB && _client._usernameB)
				printf_message(":ircserv_KAI.chat 001 " + _client._nickname + " :Welcome to the IRC Network, " + _client._nickname + " \r\n", fd);
		}
		else
			printf_message("You need more or less parameters \r\n", fd);

	}


	/*********************************************************************************/

	void	join(client _client, const std :: vector<std::string>& split_cmd ,const std::string & commandLine, int fd)
	{
		if (_client._connected)
		{
			std::cout << _client._nickname << std::endl;
			std::vector<std::string>messagesSplit = split(commandLine, ':');
			if (split_cmd[1][0]== '#' || split_cmd[1][0]== '&')
			{
				if (availableChannel(split_cmd[1]))
				{

				}

			}
		}
	}


	/*********************************************************************************/


	void	server::execute_cmd(int fd,const std :: vector<std::string>& split_cmd , int index_cmd, const std::string & commandLine)
	{
		switch (index_cmd)
		{
			case 1:
				if (split_cmd.size() > 1)
					check_password(*(this->_client[fd]), split_cmd[1], this->_password, fd);
				break;
			case 2:
				if (!alreadyUsedNickname(split_cmd[1]))
					nick(*(this->_client[fd]), split_cmd[1], fd);
				else
				{
					this->_client[fd]->_duplicateNickname = 1;
					printf_message("Error :Duplicated nickname \n\r", fd);
				}
				break;
			case 3 :
					prive_msg(*(this->_client[fd]), commandLine ,fd);
					break ;
			case 4 :
					join(*(this->_client[fd]), split_cmd, commandLine, fd);
					break ;
			default:
					break;
		}
	}

	void server::commandApply(int fd,  std::vector<std::string>& commandLine)
	{
		size_t	i;

		i = -1;
		while (++i < commandLine.size())
		{
			std::vector<std::string>firstSplit = splitBySpaces(commandLine[i]);
			for (std::vector<std::string>::iterator it = firstSplit.begin(); it !=  firstSplit.end(); ++it)
				execute_cmd(fd, firstSplit ,checkCommand(firstSplit[0]), commandLine[i]);
		}
	}




	/****************************************************/
	void server::brodcast(const std::string& msg,const std::string& channel, int fd)
	{
		unsigned int	i;

		std::map<int, client*>::iterator it = this->_client.begin();
		while(it != this->_client.end())
		{
			i = 0;
			while(i < it->second->channel.size())
			{
				if(it->second->channel[i].name == channel && fd != it->first)
					printf_message(PRIVMSG_FORMAT(this->_client[fd]->_nickname,it->second->_username,it->second->_ipclient,it->second->channel[i].name, msg),it->first);
	 			++i;
			}
			++it;
		}
	}
/**************************************************/

bool server::availableChannel(const std::string & name)
{
	unsigned	int i;
	std::map<int, client*>::iterator it;

	it  = this->_client.begin();
	while (it != _client.end())
	{
		i = 0;
		while (i < it->second->channel.size())
		{
			if(it->second->channel[i++].name == name)
				return (1);
		}
		it++;
	}
	return (0);
}



/*****************************************************/
server :: ~server()
	{
		for (std::map<int, client*>::iterator it = _client.begin(); it != _client.end(); ++it)
			delete it->second;
		this->_client.clear();
	}

