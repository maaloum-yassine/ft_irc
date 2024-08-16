/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 04:02:01 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/08/16 21:00:24 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include "Parse.hpp"

class client
{
	int					_fd;
	std::string			_nickname;
	std::string			_username;
	std::string			_realname;
	std::string			_ip;
	std::string			_hostname;

	public :
		client(int , const std::string , const std::string);

		const	std::string&		get_nickname() const;
		const	std::string&		get_username() const;
		const	std::string&		get_realname() const;
		const	std::string&		get_hostname() const;
		int							get_fd() const;

		void						set_nickname(const std::string &);
		void						set_username(const std::string &);
		void						set_realname(const std::string &);

		~client();

};



#endif
