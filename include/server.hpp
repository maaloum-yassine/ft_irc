/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:34:37 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/07/05 05:59:25 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP
#define _SERVER_HPP

#include "Parse.hpp"

class server
{

	Parse					 _parse;
	const std :: string 	_port;
	const std :: string 	_password;
	int					_serv_socket;

	public :
		server(const std::string &,const std::string &);
		void	start_server();
		~server();
};



#endif
