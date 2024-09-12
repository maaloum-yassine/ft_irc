/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_irc.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 05:37:47 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/12 08:09:29 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_IRC_HPP
#define CMD_IRC_HPP

#include "server.hpp"

	bool		alreadyUsedNickname(const std::string& );
	void		brodcast(const std::string &, const std::string &, int fd);
	bool		availableChannel(const std::string &);
	void		prive_msg(client* , const std :: vector<std::string>&, const std::string &, int );
	bool 		channelMember(const std::string&, int );
	int			searchForid(const std::string&);
	char		modeChannel(const std::string&);
	void		join(const std :: vector<std::string>& , int);
	int			idChannelfd(const std::string&, int &);
	char		memberChannelNumbers(const std::string&);
	bool		checkInvitedPersonnes(const std::string &, int, int );
	void		updateclients(const std::string & , int fd);
	std::string	clientChannels(const std::string&);
	void		displayTopic(std::string const& , std::string const&);
	std::string	topicName(const std::string& );
	void		topic(int, const std :: vector<std::string>& , const std:: string&);
	int			find_channel_id(const std::string&, int);
	bool		topicMode(const std::string&, int);
	bool 		on_channel(const std::string&, int);
	bool 		operator_user(const std::string &, int );
	void		updateChannelTopic(const std::string &, const std::string&);
	void		kick(int, const std :: vector<std::string>& , const std:: string& );
	void		kickUser(int, int,const std::string&,const std::string &);
	std::string	reason(const std::string&, int );
	void		invite(int, const std :: vector<std::string>&);
	void 		inserUser(const std::string& ,const  std::string &);
	void		part(int, const std :: vector<std::string>& );
	void		kickclients(const std::string& , int);
	void		mode(int fd, const std :: vector<std::string>&);
	int			idChannel(const std::string&, int&);
	void		updateMode(const std::string& , int , char,const std::string&);
	void		brodcastMode (const std::string&,const std::string &, int,const std::vector<std::string>& );
	void		applicateMode(char mode, std::string,int fd, char,std::vector<std::string>);
#endif // CMD_IRC_HPP
