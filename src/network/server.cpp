/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:37:50 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/08/16 23:35:27 by ymaaloum         ###   ########.fr       */
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
		this->_client[client_fd] = new client(client_fd, clientIp, clientIp);
		std::cout << "New connection from " << clientIp << std::endl;
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
			if (!poll(this->_fds.data(), this->_fds.size(), -1))
				display_err("ERROR POLL !!", 1);
			else if (this->_fds[0].events & POLLIN)
				handle_cnx_client();
			for (pllfd_itertr it = this->_fds.begin(); it != this->_fds.end(); ++it)
			{
				if (it->revents == 0)
					continue;
				if (it->revents & (POLLERR | POLLHUP))
					this->disconnection_client(it->fd);
				else if (it->revents & POLLIN)
					this->handle_msg_client(it->fd);

			}
		}
	}


	void server :: 	disconnection_client(int fd)
	{
		client* __client = NULL;

		__client = this->_client.at(fd);
		this->_client.erase(fd);
		if (__client)
		{
			for (pllfd_itertr it = this->_fds.begin(); it != this->_fds.end(); ++it)
			{
				if (it->fd == fd)
				{
					this->_fds.erase(it);
					close(fd);
					break ;
				}
			}
			std :: cout << "Client --> " << __client->get_hostname() << " is desconnected" << std :: endl;
			delete __client;
		}
	}

	void server :: handle_msg_client(int fd)
	{

		std :: string msg_env;
		char	buffer[1337];
		int		byt_rd;

		// client*	client = this->_client.at(fd);
		if ((byt_rd = recv(fd, buffer, 100, 0) < 0))
		{
			display_err("ERROR FOR READ MSG CLIENT !!", 0);
			return ;
		}
		buffer[byt_rd] = '\0';
		msg_env = std::string(buffer, byt_rd);
		std :: cout << "messgae is" << msg_env << std :: endl;
	}
	server :: ~server()
	{

	}
