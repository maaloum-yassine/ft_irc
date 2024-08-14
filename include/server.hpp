/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:34:37 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/08/14 22:40:16 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP
#define _SERVER_HPP

#include "Parse.hpp"

class server
{
	std::vector<struct pollfd>	_fds;
	Parse						_parse;
	const std :: string 		_port;
	const std :: string 		_password;
	int							_serv_socket_fd;


	public :
		server(const std::string &,const std::string &);
		void	display_err(const std ::string &)const;
		~server();
	private :
		void	start_server();
		void	create_socket();

};



#endif
