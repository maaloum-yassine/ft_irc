/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:37:50 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/08/04 20:39:59 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.hpp"

server::server(const std:: string& port, const std:: string& password):_port(port),_password(password)
{
	try
	{
		this->_parse.ParsePort(trim(_port));
		this->start_server();

	}
	catch(const std::string& err)
	{
		std :: cerr << err << std :: endl;
		exit (EXIT_FAILURE);
	}
}


	void server :: 	start_server()
	{
		_serv_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (_serv_socket == -1)
			throw std :: string ("ERROR CREATING SOCKET!");

	}

server :: ~server()
{

}
