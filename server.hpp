/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:34:37 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/07/05 04:42:15 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP
#define _SERVER_HPP

#include "./Parse/Parse.hpp"

class server
{

	Parse					 _parse;
	const std :: string 	_port;
	const std :: string 	_password;

	public :
		server(const std::string &,const std::string &);
		~server();
};



#endif
