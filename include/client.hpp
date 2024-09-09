/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 04:02:01 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/09 08:43:32 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include "Parse.hpp"

class client
{
	public :
		int 					_fd;
		std::string 			_ipclient;
		std::string 			_nickname;
		std::string 			_username;
		std::vector<channels> 	channel;
		bool 					_passB;
		bool 					_nicknameB;
		bool 					_usernameB;
		bool 					_connected;
		bool 					_duplicateNickname;
		client(int , const std::string &);
		void		addFd(int );
		void		addNickname(std::string const &);
		void		addUser(std::string const &);
		~client();

};



#endif
