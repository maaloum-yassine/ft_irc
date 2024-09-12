/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_irc.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 05:46:09 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/12 07:26:43 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cmd_irc.hpp"

	void	check_password(client *_client ,const std::string & pass, const std::string& pass_serv, int fd)
	{
		if (pass == pass_serv)
		{
			printf_message("Correct password\n\r", fd);
			_client->_passB = 1;
			std::cout << "passb" << _client->_passB << std::endl;
			std::cout << "nicknameb" << _client->_nicknameB<< std::endl;
			std::cout << "usernameB" << _client->_usernameB<< std::endl;
			if(_client->_passB && _client->_nicknameB && _client->_usernameB)
			{
				std::string msg = ":ircserv_KAI->chat 001 " + _client->_nickname + " :Welcome to the IRC Network, " + _client->_nickname + " \r\n";
				printf_message(msg, fd);
			}
		}
		else
			printf_message("Wrong password\n", fd);
	}
// 	/********************************************************************************/

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

	void	nick(client* _client ,const std::string & _nicknm, int fd)
	{
		_client->addNickname(_nicknm);
		printf_message(RPL_NICK_SET(_client->_ipclient, _nicknm), fd);
		_client->_nicknameB = 1;
		if(_client->_passB && _client->_nicknameB && _client->_usernameB)
			printf_message( ":ircserv_KAI->chat 001 " + _client->_nickname + " :Welcome to the IRC Network, " + _client->_nickname + " \r\n", fd);
	}
// 	/********************************************************************************/


	void	user(client *_client, const std:: string& commandLine,int fd)
	{

		bool	tr;

		tr = 0;
		std::vector<std::string>username = split(commandLine, ':');
		if(count_words(commandLine, tr) == 5 && tr)
		{
			std::cout << username[1] << std::endl;
			_client->addUser(username[1]);
			_client->_usernameB = 1;
			if(_client->_passB && _client->_nicknameB && _client->_usernameB)
				printf_message(":ircserv_KAI->chat 001 " + _client->_nickname + " :Welcome to the IRC Network, " + _client->_nickname + " \r\n", fd);
		}
		else
			printf_message("You need more or less parameters \r\n", fd);
	}


// 	/*********************************************************************************/

	void	server ::prive_msg(client* _client, const std :: vector<std::string>& split_cmd ,const std::string & commandLine, int fd)
	{
		if (_client->_connected)
		{
			std::cout << _client->_nickname << std::endl;
			std::vector<std::string>messagesSplit = split(commandLine, ':');
			if (split_cmd[1][0]== '#' || split_cmd[1][0]== '&')
			{
				if (availableChannel(split_cmd[1]))
				{
					if (channelMember(split_cmd[1], fd))
						brodcast(messagesSplit[1], split_cmd[1], fd);
					else
						printf_message(ERR_CANNOTSENDTOCHAN(_client->_ipclient,_client->_nickname, commandLine),fd);
				}
				else
					printf_message(ERR_CANNOTSENDTOCHAN(_client->_ipclient,_client->_nickname, split_cmd[1]),fd);
			}
			else
			{
				if(split_cmd.size() == 1)
						printf_message(ERR_NORECIPIENT(_client->_ipclient, _client->_nickname, split_cmd[0]),fd);
					else if(split_cmd.size() == 2)
						printf_message(ERR_NOTEXTTOSEND(_client->_ipclient, _client->_nickname), fd);
					else
					{
						int _fd = searchForid(split_cmd[1]);
						if(_fd != -1)
							printf_message(PRIVMSG_FORMAT(_client->_nickname , _client->_username, _client->_ipclient, split_cmd[1] , messagesSplit[1]), _fd);
						else
							printf_message(ERR_NOSUCHNICK(_client->_ipclient,_client->_nickname),fd);
					}
			}
		}
	}



// 	/*********************************************************************************/


	char	server::modeChannel(const std::string& name)
	{
		unsigned int i;
		std::map<int, client*>::iterator it  = this->_client.begin();

		while (it !=  this->_client.end())
		{
			i = 0;
			while (i < it->second->channel.size())
			{
				if(it->second->channel[i].name == name)
					return (it->second->channel[i].mode);
				++i;
			}
			++it;
		}
		return (0);
	}

// 	/*********************************************************************************/

	int 	server::idChannelfd(const std::string& name, int &fd)
	{

		std::map<int, client*>::iterator it  = this->_client.begin();
		unsigned	int	i;


		while(it != this->_client.end())
		{
			i = 0;
			while (i < it->second->channel.size())
			{
				if(it->second->channel[i].name == name)
				{
					fd = it->first;
					return i;
				}
				i++;
			}
			it++;
		}
		return (-1);
	}


char		server::memberChannelNumbers(const std::string& name)
{
	std::map<int, client*>::iterator it  = this->_client.begin();
	unsigned int i;
	int number;

	number = 0;

	while (it != _client.end())
	{
		i = 0;
		while (i < it->second->channel.size())
		{
			if(it->second->channel[i].name == name)
			{
				std::cout << "id " << it->first << number << std::endl;
				number++;
			}
			i++;
		}
		it++;
	}
	std::cout << "number " << number << std::endl;
	return (number);
}



	bool server::checkInvitedPersonnes(const std::string& name, int channelid, int fd)
	{
		unsigned	int	i;
		i = 0;

		while(i < this->_client[fd]->channel[channelid].invited.size())
		{
			if(this->_client[fd]->channel[channelid].invited[i] == name)
				return (1);
			i++;
		}
		return (0);
	}


	std::string server::clientChannels(const std::string& channel)
	{
		std::string contenated = "";
		std::map<int, client*>::iterator it = this->_client.begin();
		unsigned	int	i;
		bool			b;

		b = 0;
		while (it != this->_client.end())
		{
			i = 0;
			while (i < it->second->channel.size())
			{
				if (it->second->channel[i].name == channel)
				{
					if (!b)
						contenated += adminOrNot(it->second->channel[i].op) +  it->second->_nickname;
					else
						contenated += " " +adminOrNot(it->second->channel[i].op) +  it->second->_nickname;
					b = 1;
					break;
				}
				i++;
			}
			++it;
		}
		return contenated;
	}


	void		server::updateclients(const std::string& channel , int fd)
	{
		unsigned	int		i;

		std::map<int, client*>::iterator it = this->_client.begin();
		while (it != this->_client.end())
		{
			i = 0;
			while (i < it->second->channel.size())
			{
				if (it->second->channel[i++].name == channel)
					printf_message(RPL_NAMREPLY(this->_client[fd]->_ipclient, clientChannels(channel)  ,  channel, this->_client[fd]->_nickname) , it->first);
			}
			++it;
		}
	}


	void		server::displayTopic(const std::string& topic, const std::string& channelname)
	{
		std::map<int, client*>::iterator it = this->_client.begin();

		unsigned	int i;
		std::cout << "topic name " << topic << std::endl;
		std::cout << "channel " << channelname << std::endl;
		std::cout << "topic" << topic;
		while(it != this->_client.end())
		{
			i = 0;
			while (i < it->second->channel.size())
			{
				if (it->second->channel[i].name == channelname)
				{
					printf_message(RPL_TOPIC(it->second->_ipclient, it->second->_nickname, channelname, topic),it->first);
				}
				++i;
			}
			++it;
		}
	}

	std::string		server::topicName(const std::string& channelname)
	{
		unsigned int i;

		std::map<int, client*>::iterator it = this->_client.begin();

		while(it != this->_client.end())
		{
			i = 0;
			while(i < it->second->channel.size())
			{
				if(it->second->channel[i].name == channelname)
					return it->second->channel[i].topic;
				++i;
			}
			++it;
		}
		return "";
	}



	void	server::join(const std :: vector<std::string>& split_cmd , int fd)
	{
		unsigned	int		i;
		char				mode;

		i = 0;
		mode = 0;
		if (split_cmd.size() != 2)
			printf_message( "More or less argument\n\r" , fd);
		else
		{
			std :: cout << "/*/*/**" << std :: endl;
			std::vector<std::string>channelSplited = split(split_cmd[1], ',');
			while (i < channelSplited.size())
			{
				if (checkChannelName(channelSplited[i]))
				{
					if (availableChannel(channelSplited[i]))
					{
						mode = modeChannel(channelSplited[i]);
						if (mode & (1 << LIMIT))
						{
							int id;
							int _fd;
							id = idChannelfd(channelSplited[i], _fd);
							if (memberChannelNumbers(channelSplited[i]) < this->_client[_fd]->channel[id].limit)
							{
								std::cout << "1333337" << std::endl;
								std::cout << "bnumber" << memberChannelNumbers(channelSplited[i]) << std::endl;
								std::cout << "limit" << this->_client[_fd]->channel[id].limit<< std::endl;
								if (mode & (1 << INVITE_ONLY) && checkInvitedPersonnes (this->_client[fd]->_nickname, id, _fd))
								{
									if(mode & (1 << KEY) && split_cmd.size() >2)
									{
 										std::vector<std::string>keySplited = split(split_cmd[1], ',');
										if(i < split_cmd.size() && keySplited[i] == this->_client[_fd]->channel[id].key)
										{
											this->_client[fd]->channel.push_back(channels(channelSplited[i], modeChannel(channelSplited[i]), 0, this->_client[_fd]->channel[id].invited));
											printf_message(RPL_JOIN(this->_client[fd]->_nickname, this->_client[fd]->_username, channelSplited[i], this->_client[fd]->_ipclient) , fd);
											updateclients(channelSplited[i], fd);
										// message(RPL_NAMREPLY(this->_client[fd].ipclient, clientChannels(channelSplited[i])  , channelSplited[i], this->_client[fd].nickname) , fd);
										}
										else
											std::cout << "hnaNOO" << std::endl;
									}
									else if(mode & (1 << KEY) && split_cmd.size() <= 2)
											std::cout << "--NOO" << std::endl;
									else
									{
										this->_client[fd]->channel.push_back(channels(channelSplited[i], modeChannel(channelSplited[i]), 0, this->_client[_fd]->channel[id].invited));
										printf_message(RPL_JOIN(this->_client[fd]->_nickname, this->_client[fd]->_username, channelSplited[i], this->_client[fd]->_ipclient) , fd);
										// message(RPL_NAMREPLY(this->_client[fd]->ipclient, clientChannels(channelSplited[i])  , channelSplited[i], this->_client[fd]->nickname) , fd);
										updateclients(channelSplited[i], fd);
										displayTopic(topicName(channelSplited[i]), channelSplited[i]);
									}
								}
							}
							else
								printf_message(ERR_CHANNELISFULL(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, channelSplited[i]), fd);
						}
						else
						{
							int id;
							int _fd;
							id = idChannelfd(channelSplited[i], _fd);
							std::cout << "2" << std::endl;
							if (mode & (1 << INVITE_ONLY) && checkInvitedPersonnes(this->_client[fd]->_nickname, id, _fd))
							{
								std::cout << "INVITE ONLY" << std::endl;
								this->_client[fd]->channel.push_back(channels(channelSplited[i], modeChannel(channelSplited[i]), 0, this->_client[_fd]->channel[id].invited));
								printf_message(RPL_JOIN(this->_client[fd]->_nickname, this->_client[fd]->_username, channelSplited[i], this->_client[fd]->_ipclient), fd);
								// message(RPL_NAMREPLY(this->_client[fd].ipclient, clientChannels(channelSplited[i])  , channelSplited[i], this->_client[fd]->nickname) , fd);
								updateclients(channelSplited[i], fd);
								displayTopic(topicName(channelSplited[i]), channelSplited[i]);
							}
							else if(!(mode & (1 << INVITE_ONLY)))
							{
								if(mode & (1 << KEY) && split_cmd.size() > 2)
								{
									std::cout << "key ONLY" << std::endl;
									std::vector<std::string>keySplited = split(split_cmd[2], ',');
									if (i <= keySplited.size() && keySplited[i] == this->_client[_fd]->channel[id].key)
									{
										this->_client[fd]->channel.push_back(channels(channelSplited[i], modeChannel(channelSplited[i]), 0, this->_client[_fd]->channel[id].invited));
										printf_message(RPL_JOIN(this->_client[fd]->_nickname, this->_client[fd]->_username, channelSplited[i], this->_client[fd]->_ipclient), fd);
										// printf_message(RPL_NAMREPLY(this->_client[fd].ipclient, clientChannels(channelSplited[i])  , channelSplited[i], this->_client[fd].nickname) , fd);
										updateclients(channelSplited[i], fd);
										displayTopic(topicName(channelSplited[i]), channelSplited[i]);
									}
									else
										printf_message("Wrong password\n\r" , fd);
								}
								else if(mode & (1 << KEY) && split_cmd.size() <= 2)
										printf_message(ERR_BADCHANNELKEY(this->_client[fd]->_nickname,this->_client[fd]->_ipclient,channelSplited[i]),fd);
								else
								{
									std::cout << "no ONLY" << std::endl;
									this->_client[fd]->channel.push_back(channels(channelSplited[i], modeChannel(channelSplited[i]), 0, this->_client[_fd]->channel[id].invited));
									printf_message(RPL_JOIN(this->_client[fd]->_nickname, this->_client[fd]->_username, channelSplited[i], this->_client[fd]->_ipclient), fd);
									// printf_message(RPL_NAMREPLY(this->_client[fd].ipclient, clientChannels(channelSplited[i])  , channelSplited[i], this->_client[fd].nickname) , fd);
									updateclients(channelSplited[i], fd);
									displayTopic(topicName(channelSplited[i]), channelSplited[i]);
								}
							}
							else
							{
								printf_message(ERR_INVITEONLY(this->_client[fd]->_nickname, this->_client[fd]->_ipclient, channelSplited[i]), fd);
							}
						}
					}
					else
					{
						std::cout << "3" << std::endl;
						std::vector<std::string> invited;
						this->_client[fd]->channel.push_back(channels(channelSplited[i], 1 << TOPIC, 1, invited));
						printf_message(RPL_JOIN(this->_client[fd]->_nickname, this->_client[fd]->_username, channelSplited[i], this->_client[fd]->_ipclient) ,fd);
						printf_message(RPL_NAMREPLY(this->_client[fd]->_ipclient, clientChannels(channelSplited[i]), channelSplited[i], this->_client[fd]->_nickname), fd);
						displayTopic(topicName(channelSplited[i]), channelSplited[i]);
					}
				}
				else
				{
					printf_message(ERR_NOSUCHCHANNEL(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, channelSplited[i]), fd);
					std::map<int, client*>::iterator it = _client.begin();
					while(it != _client.end())
					{
						unsigned	int i;

						i = 0;
						while(i < it->second->channel.size())
						{
							unsigned	int j = 0;
							while(j < it->second->channel[i].invited.size())
								std::cout << it->second->channel[i].invited[j++] << std::endl;
							++i;
						}
						++it;
					}
				}
				i++;
			}
		}
	}

// 	/*********************************************************************************/
	bool server::topicMode(const std::string& name, int fd)
	{
		int	pos;

		pos = find_channel_id(name, fd);
		if(pos != -1)
		{
			if(!(this->_client[fd]->channel[pos].mode & (1 << TOPIC)))
				return (1);
			return(0);
		}
		return(0);
	}
// 	/**********************************************************************************/
	int	server::find_channel_id(const std::string& name, int fd)
	{
		unsigned int i;

		i = 0;
		while (i < this->_client[fd]->channel.size())
		{
			if ( this->_client[fd]->channel[i].name == name)
				return (i);
			i++;
		}
		return (-1);
	}


	bool server::on_channel(const std::string& name, int fd)
	{
		unsigned	int i;

		i = 0;
		while (i < this->_client[fd]->channel.size())
		{
			if (this->_client[fd]->channel[i].name == name)
				return (1);
			i++;
		}
		return (0);
	}




	bool server::operator_user(const std::string& name, int fd)
	{
		int pos;

		pos = find_channel_id(name, fd);
		if(pos != -1)
		{
			if(this->_client[fd]->channel[pos].op)
				return (1);
			return(0);
		}
		return(0);
	}




	void		server::updateChannelTopic(const  std::string& topic,const  std::string& channelname)
	{
		std::map<int, client*>::iterator it = this->_client.begin();

		unsigned	int i;
		while(it != this->_client.end())
		{
			i = 0;
			while(i < it->second->channel.size())
			{
				if(it->second->channel[i].name == channelname)
				{
					it->second->channel[i].topic = topic;
					break;
				}
				i++;
			}
			it++;
		}
	}



// 	/************************************************************************************/
	void	server::topic(int fd, const std :: vector<std::string>& split_cmd, const std:: string& commandLine)
	{
		std :: cout << "*-*-*-" <<std :: endl;
		int error_ch = 0;
		int error_rights = 0;
		int topic = 0;


		error_ch = on_channel(split_cmd[1],fd);
		error_rights = operator_user(split_cmd[1], fd);
		topic = topicMode(split_cmd[1], fd);
		if(error_ch && (error_rights || topic))
		{
			std::vector<std::string>messagesSplit = split(commandLine, ':');
			if(messagesSplit.size() == 2)
			{
				updateChannelTopic(messagesSplit[1], split_cmd[1]);
				displayTopic(messagesSplit[1], split_cmd[1]);
			}
			else if(messagesSplit.size() == 1)
			{
				printf_message(RPL_TOPIC(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, split_cmd[1],topicName(split_cmd[1])),fd);
				std::cout << topicName(split_cmd[1]) << std::endl;
				std::cout << split_cmd[1] << std::endl;
			}
		}
		else
		{
			if(error_ch == 0)
				printf_message(ERR_NOTONCHANNEL(this->_client[fd]->_ipclient, split_cmd[1]), fd);
			else if(error_ch == 1 && error_rights == 0)
				printf_message(ERR_CHANOPRIVSNEEDED(this->_client[fd]->_ipclient,this->_client[fd]->_nickname,split_cmd[1]), fd);
			}

	}


// 	/******************************************************************************* */

	std::string server::reason(const std::string& str, int fd)
	{
		std::vector<std::string> splitedString;
		std::string line = this->_client[fd]->_nickname;
		splitedString = split(str,':');
		if(splitedString.size() > 1)
			line = splitedString[1];
		return line;
	}



	void server::kickUser(int fd, int index, const std::string name,const std::string reason)
	{
		bool b;
		unsigned int i;
		i = 0;
		b = 0;
		std::map<int, client*>::iterator it = this->_client.begin();
		while (i < this->_client[index]->channel.size())
		{
			if (this->_client[index]->channel[i].name == name)
			{
				b = 0;
				unsigned int j;
				while(it != this->_client.end())
				{
					j = 0;
					while(j < it->second->channel.size())
					{
						if(it->second->channel[j].name == name)
							printf_message(RPL_KICK(this->_client[fd]->_nickname, this->_client[fd]->_username,this->_client[fd]->_ipclient,name,this->_client[index]->_nickname,reason),it->first);
						j++;
					}
					it++;
				}
				this->_client[index]->channel.erase(this->_client[index]->channel.begin() + i);
				break;
			}
			i++;
		}
		if(b == 0)
			printf_message(ERR_NOSUCHNICK(this->_client[fd]->_ipclient, this->_client[fd]->_nickname), fd);
	}

	void server::kick(int fd, const std :: vector<std::string>& split_cmd, const std:: string& commandLine)
	{
		std::string line = "";

		if(split_cmd.size() < 2)
			ERR_NEEDMOREPARAMS(this->_client[fd]->_nickname ,this->_client[fd]->_ipclient,split_cmd[0]);
		else
		{
			if(operator_user(split_cmd[1], fd))
			{
				int index = searchForid(split_cmd[2]);
				if(index == -1)
					printf_message(ERR_NOSUCHNICK(this->_client[fd]->_ipclient,this->_client[fd]->_nickname),fd);
				else
					kickUser(fd, index, split_cmd[1], reason(commandLine, fd));
			}
			else
				printf_message(ERR_CHANOPRIVSNEEDED(this->_client[fd]->_ipclient,this->_client[fd]->_nickname,split_cmd[1]),fd);
		}
	}

// 	/*********************************************************************************/




	void server::inserUser(const std::string& nickname, const std::string& channel)
	{
		std::map<int, client* >::iterator it = this->_client.begin();

		unsigned	int	i;
		while(it != this->_client.end())
		{
			i = 0;
			while(i < it->second->channel.size())
			{
				if(it->second->channel[i].name == channel)
					it->second->channel[i].invited.push_back(nickname);
				++i;
			}
			++it;
		}
	}

	void	server::	invite(int fd, const std :: vector<std::string>& split_cmd)
	{
		if(split_cmd.size() < 2)
			ERR_NEEDMOREPARAMS(this->_client[fd]->_nickname ,this->_client[fd]->_ipclient,split_cmd[0]);
		else
		{
			if (split_cmd.size() < 3)
				printf_message(ERR_NEEDMOREPARAMS(this->_client[fd]->_nickname, this->_client[fd]->_ipclient , split_cmd[0]), fd);
			int index = -1;
			index = searchForid(split_cmd[1]);
			std::cout << "fs1 " << split_cmd[1] << std::endl;
			std::cout << "fs2 " << split_cmd[2] << std::endl;
			if (availableChannel(split_cmd[2]))
			{
				if (!on_channel(split_cmd[1], fd))
				{
					if (operator_user(split_cmd[2], fd))
					{
						if (index == -1)
							printf_message(ERR_NOSUCHNICK(this->_client[fd]->_ipclient,this->_client[fd]->_nickname), fd);
						else
						{
							printf_message(RPL_INVITING(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, split_cmd[1], split_cmd[2]), fd);
							printf_message(RPL_INVITE(this->_client[fd]->_nickname, this->_client[fd]->_username, this->_client[fd]->_ipclient, split_cmd[1], split_cmd[2]), index);
							inserUser(split_cmd[1], split_cmd[2]);
						}
					}
					else
						printf_message(ERR_CHANOPRIVSNEEDED(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, split_cmd[1]), fd);
				}
				else
					printf_message(ERR_USERONCHANNEL(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, split_cmd[1], split_cmd[2]), fd);
			}
			else
				printf_message(ERR_NOSUCHCHANNEL(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, split_cmd[2]),fd);
		}
	}

// 	/*********************************************************************************/



	void		server::kickclients(const	std::string &channel , int fd)
	{
		unsigned	int i;

		std::map<int, client*>::iterator it = this->_client.begin();
		while(it != this->_client.end())
		{
			i = 0;
			while(i < it->second->channel.size())
			{
				if(it->second->channel[i].name == channel)
					printf_message(RPL_KICK(_client[fd]->_nickname, _client[fd]->_username, _client[fd]->_ipclient, channel, _client[fd]->_nickname, ""),it->first);
				++i;
			}
			++it;
		}
	}



	void	server::	part(int fd, const std :: vector<std::string>& split_cmd)
	{

		if (split_cmd.size() == 3)
		{
			unsigned	int j = 0;
			while (j < this->_client[fd]->channel.size())
			{
				if (this->_client[fd]->channel[j].name == split_cmd[1])
				{
					this->_client[fd]->channel.erase(this->_client[fd]->channel.begin() + j);
					printf_message(RPL_QUIT(this->_client[fd]->_nickname, this->_client[fd]->_nickname, split_cmd[2]), fd);
					kickclients(split_cmd[1], fd);
					break;
				}
				++j;
			}
		}
	}




void server::brodcastMode (const std::string& channel,const  std::string& mode, int fd,const  std::vector<std::string>& args)
{
	unsigned int i;

	std::string msg = "";
	std::string arg = "";
	i = 0;

	if (mode[1] == 'k')
		arg = args[0];
	else if (mode[1] == 'l')
		arg = args[1];
	else
		arg = args[2];

	std::map<int, client*>::iterator it = this->_client.begin();
	if (mode[0] == '+')
		msg = RPL_ADDMODE(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, channel, mode ,arg, this->_client[fd]->_username);
	else if (mode[0] == '-' && mode[1] != 'o')
		msg = RPL_DELMODE(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, channel, mode , this->_client[fd]->_username);
	else if (mode[0] == '-' && mode[1] == 'o')
		msg = RPL_DELOP(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, channel, mode , args[2]);
	while (it != this->_client.end())
	{
		i = 0;
		while(i < it->second->channel.size())
		{
			if(it->second->channel[i].name == channel)
		 		printf_message(msg, it->first);
			++i;
		}
		++it;
	}
}


void	server::updateMode (const std::string& channel, int wich, char sign, const std::string& arg)
{
	std::map<int, client *>::iterator it = this->_client.begin();
	unsigned	int i;

	while (it != this->_client.end())
	{
		i = 0;
		while (i < it->second->channel.size())
		{
			if (it->second->channel[i].name == channel)
			{
				if(wich == KEY)
					it->second->channel[i].key = arg;
				else if(wich == LIMIT)
				{
					const char* c_str = arg.c_str();
					it->second->channel[i].limit = convert((char*)  c_str);
				}
				if(sign == '+')
					it->second->channel[i].mode |= (1 << wich);
				else
					it->second->channel[i].mode &= ~(1 << wich);
			}
			++i;
		}
		++it;
	}
}




void	server::applicateMode(char mode, std::string channel, int fd, char used, std::vector<std::string> args)
{
	std::map<int, client*>::iterator it = this->_client.begin();

	unsigned int i;
	while (it != this->_client.end())
	{
		i = 0;
		while (i < it->second->channel.size())
		{
			if (it->second->channel[i].name == channel)
			{
				if ((mode & (1 << INVITE_ONLY)) && (it->second->channel[i].mode & (1 << INVITE_ONLY)) && used & (1 << INVITE_ONLY))
					std::cout << "already INVITE " << std::endl;
				else if ((mode & (1 << INVITE_ONLY)) && !(it->second->channel[i].mode & (1 << INVITE_ONLY)) && used & (1 << INVITE_ONLY))
				{
					updateMode(channel, INVITE_ONLY, '+', "");
					brodcastMode(channel, "+i", fd, args);
					std::cout << "INVITE " << std::endl;
				}
				else if (!(mode & (1 << INVITE_ONLY)) && (it->second->channel[i].mode & (1 << INVITE_ONLY)) && used & (1 << INVITE_ONLY))
				{
					updateMode(channel, INVITE_ONLY, '-', "");
					brodcastMode(channel, "-i", fd, args);
					std::cout << "DELETE INVITE " << std::endl;
				}
				else if (!(mode & (1 << INVITE_ONLY)) && !(it->second->channel[i].mode & (1 << INVITE_ONLY)) && (used & (1 << INVITE_ONLY)))
				{
					std::cout << "cannot DELETE invite " << std::endl;
				}

				if ((mode & (1 << TOPIC)) && (it->second->channel[i].mode & (1 << TOPIC)) && (used & (1 << TOPIC)))
				{
					std::cout << "already topic " << std::endl;
				}
				else if ((mode & (1 << TOPIC)) && !(it->second->channel[i].mode & (1 << TOPIC)) && used & (1 << TOPIC))
				{
					updateMode(channel, TOPIC, '+', "");
					brodcastMode(channel, "+t", fd, args);
					std::cout << "INVITE topic" << std::endl;
				}
				else if (!(mode & (1 << TOPIC)) && (it->second->channel[i].mode & (1 << TOPIC)) && used & (1 << TOPIC))
				{
					updateMode(channel, TOPIC, '-', "");
					brodcastMode(channel, "-t", fd, args);
					std::cout << "DELETE INVITE topic " << std::endl;
				}
				else if (!(mode & (1 << TOPIC)) && !(it->second->channel[i].mode & (1 << TOPIC)) && used & (1 << TOPIC))
					std::cout << "cannot DELETE topic " << std::endl;

				if (!args[0].empty())
				{
					if ((mode & (1 << KEY)) && (it->second->channel[i].mode & (1 << KEY)))
						std::cout << "already key" << std::endl;
					else if ((mode & (1 << KEY)) && !(it->second->channel[i].mode & (1 << KEY)))
					{
						updateMode(channel, KEY, '+', args[0]);
						brodcastMode(channel, "+k", fd, args);
						std::cout << "INVITE key" << std::endl;
					}
					else if (!(mode & (1 << KEY)) && (it->second->channel[i].mode & (1 << KEY)))
					{
						if (it->second->channel[i].key == args[0])
						{
							updateMode(channel, KEY, '-', "");
							brodcastMode(channel, "-k", fd, args);
							std::cout << "DELETE key " << std::endl;
						}
						else
							std::cout << "invalid key " << std::endl;
					}
					else if (!(mode & (1 << KEY)) && !(it->second->channel[i].mode & (1 << KEY)))
						std::cout << "cannot DELETE key " << std::endl;
				}

				if (!args[1].empty() && limitNumber(args[1]))
				{
					if ((mode & (1 << LIMIT)) && (it->second->channel[i].mode & (1 << LIMIT)))
						std::cout << "already limit" << std::endl;
					else if ((mode & (1 << LIMIT)) && !(it->second->channel[i].mode & (1 << LIMIT)))
					{
						const char* c_str = args[1].c_str();
						int num = convert((char*)c_str);
						std::stringstream ss;
						ss << num;
						std::string str = ss.str();
						args[1] = str;
						updateMode(channel, LIMIT, '+', args[1]);
						brodcastMode(channel, "+l", fd, args);
						std::cout << "INVITE limit" << std::endl;
					}
					else if (!(mode & (1 << LIMIT)) && (it->second->channel[i].mode & (1 << LIMIT)))
					{
						const char* c_str = args[1].c_str();

						if (convert((char*)c_str) == it->second->channel[i].limit)
						{
							updateMode(channel, LIMIT, '-', args[1]);
							std::stringstream ss;
							ss << c_str;
							std::string str = ss.str();
							args[1] = str;
							brodcastMode(channel, "-l", fd, args);
							std::cout << "DELETE limit " << std::endl;
						}
					}
					else if (!(mode & (1 << LIMIT)) && !(it->second->channel[i].mode & (1 << LIMIT)))
						std::cout << "cannot DELETE limit " << std::endl;
				}

				if (!args[2].empty())
				{
					int f = searchForid(args[2]);
					int b = 0;
					unsigned int k = 0;
					while (k < this->_client[f]->channel.size())
					{
						if (this->_client[f]->channel[k].name == it->second->channel[i].name)
						{
							b = 1;
							break;
						}
						k++;
					}

					if (b)
					{
						if ((mode & (1 << OPERATOR)) && (this->_client[f]->channel[k].op == 1))
						{
							std::cout << "nickname" << it->second->_nickname << std::endl;
							std::cout << "op" << it->second->channel[i].op << std::endl;
							std::cout << "already operator" << std::endl;
						}
						else if ((mode & (1 << OPERATOR)) && (this->_client[f]->channel[k].op == 0))
						{
							this->_client[f]->channel[k].op = 1;
							brodcastMode(channel, "+o", fd, args);
							updateclients(channel, fd);
							std::cout << "set operator" << std::endl;
						}
						else if (!(mode & (1 << OPERATOR)) && (this->_client[f]->channel[k].op == 1))
						{
							this->_client[f]->channel[k].op = 0;
							brodcastMode(channel, "-o", fd, args);
							updateclients(channel, fd);
							std::cout << "DELETE operator " << std::endl;
						}
						else if (!(mode & (1 << OPERATOR)) && (this->_client[f]->channel[k].op == 0))
						{
							std::cout << "cannot DELETE operator" << std::endl;
						}
					}
				}
				return;
			}
			i++;
		}
		it++;
	}
}

// 	/***************************************************************************************/

	int  server::idChannel(const  std::string& name, int& fd)
	{

		unsigned int i;
		i = 0;

		while (i < this->_client[fd]->channel.size())
		{
			if(this->_client[fd]->channel[i].name == name)
				return i;
			i++;
		}
		if(availableChannel(name))
			return (-1);
		else
			return (-2);
	}


// 	/**********************************************************************************/
	void server::mode(int fd, const std::vector<std::string>& split_cmd) {
		// Création d'une map pour stocker les modes
		std::map<int, int> modes;

		// Vérification du nombre d'arguments
		if (split_cmd.size() < 3) {
			// Envoi d'un message d'erreur si le nombre d'arguments est insuffisant
			printf_message(ERR_NEEDMOREPARAMS(this->_client[fd]->_nickname, this->_client[fd]->_ipclient, split_cmd[0]), fd);
		} else {
			// Déclaration de variables pour le traitement des modes
			int pos;
			pos = idChannel(split_cmd[1], fd); // Récupération de l'identifiant du canal
			if (pos == -1) {
				// Erreur si le canal n'est pas trouvé
				printf_message(ERR_NOTONCHANNEL(this->_client[fd]->_ipclient, split_cmd[1]), fd);
				return;
			}
			if (pos == -2) {
				// Erreur si le canal n'existe pas
				printf_message(ERR_NOSUCHCHANNEL(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, split_cmd[1]), fd);
				return;
			}
			if (this->_client[fd]->channel[pos].op != 1) {
				// Erreur si le client n'est pas opérateur sur le canal
				printf_message(ERR_NOTOP(this->_client[fd]->_ipclient, split_cmd[1]), fd);
				return;
			}

			// Initialisation des variables pour les modes
			std::vector<std::string> args(3);
			unsigned int i;
			int j = 0;
			int size = 0;
			int oi = 0;
			char mode = 0;
			mode = 1 << POSITIF; // Initialisation du mode
			char used = 0;
			bool enter = 0;
			i = 2;
			j = 0;
			unsigned int pkey = -1;

			// Parcours des arguments pour les modes
			while (i < split_cmd.size()) {
				j = 0;
				size = split_cmd[i].size();
				oi = i;
				while (j < size) {
					// Vérification des caractères valides pour les modes
					if (check_arg([oi][j]))
						printf_message(ERR_UNKNOWNMODE(this->_client[fd]->_ipclient, this->_client[fd]->_nickname, split_cmd[oi][j]), fd);
					else if (split_cmd[oi][j] == '-' || split_cmd[oi][j] == '+') {
						// Gestion des signes '+' et '-'
						if (split_cmd[oi][j] == '-') {
							mode &= ~(1 << POSITIF);
						} else {
							mode |= (1 << POSITIF);
						}
					} else if (split_cmd[oi][j] == 'i') {
						// Gestion du mode INVITE_ONLY
						used |= 1 << INVITE_ONLY;
						enter = 1;
						if (mode & (1 << POSITIF)) {
							mode |= 1 << INVITE_ONLY;
						} else {
							mode &= ~(1 << INVITE_ONLY);
						}
						mode |= 1 << POSITIF;
					} else if (split_cmd[oi][j] == 't') {
						// Gestion du mode TOPIC
						used |= 1 << TOPIC;
						enter = 1;
						if (mode & (1 << POSITIF)) {
							mode |= 1 << TOPIC;
						} else {
							mode &= ~(1 << TOPIC);
						}
						mode |= 1 << POSITIF;
					} else if (split_cmd[oi][j] == 'k' || split_cmd[oi][j] == 'o' || split_cmd[oi][j] == 'l') {
						// Gestion des modes KEY, OPERATOR, LIMIT
						enter = 1;
						i++;
						while (i <= pkey)
							i++;
						if (split_cmd[oi][j] == 'k') {
							if (mode & (1 << POSITIF)) {
								mode |= 1 << KEY;
							} else {
								mode &= ~(1 << KEY);
							}
						} else if (split_cmd[oi][j] == 'o') {
							if (mode & (1 << POSITIF)) {
								mode |= 1 << OPERATOR;
							} else {
								mode &= ~(1 << OPERATOR);
							}
						} else if (split_cmd[oi][j] == 'l') {
							used |= 1 << LIMIT;
							if (mode & (1 << POSITIF)) {
								mode |= 1 << LIMIT;
							} else {
								mode &= ~(1 << LIMIT);
							}
						}
						if (i < split_cmd.size()) {
							if (split_cmd[oi][j] == 'k') {
								used |= 1 << KEY;
								args[0] = split_cmd[i];
								std::cout << "key " << split_cmd[i] << std::endl;
							} else if (split_cmd[oi][j] == 'l') {
								used |= 1 << LIMIT;
								args[1] = split_cmd[i];
								std::cout << args[1] << std::endl;
								std::cout << "limit " << split_cmd[i] << std::endl;
							} else if (split_cmd[oi][j] == 'o') {
								used |= 1 << OPERATOR;
								args[2] = split_cmd[i];
							}
						} else {
							if (split_cmd[oi][j] == 'k')
								args[0] = "";
							else if (split_cmd[oi][j] == 'l')
								args[1] = "";
							else if (split_cmd[oi][j] == 'o')
								args[2] = "";
						}
						pkey = i;
						mode |= 1 << POSITIF;
					}
					j++;
				}
				i++;
			}

			// Application des modes si nécessaire
			if (enter)
				applicateMode(mode, split_cmd[1], fd, used, args);
		}
	}


	/*********************************************************************************/

	void server::commandApply(int fd,  std::vector<std::string>& commandLine)
	{
		size_t	i;

		i = -1;
		while (++i < commandLine.size())
		{
			std::vector<std::string>firstSplit = splitBySpaces(commandLine[i]);
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
// /**************************************************/

bool server::availableChannel(const std::string & name_channel)
{

	unsigned	int i;
	std::map<int, client*>::iterator it;

	it  = this->_client.begin();
	while (it != _client.end())
	{
		i = 0;
		while (i < it->second->channel.size())
		{
			if(it->second->channel[i++].name == name_channel)
				return (1);
		}
		++it;
	}
	return (0);
}




bool server::channelMember(const std::string& channel, int fd)
{
	unsigned int i;

 	i = 0;

	while (i < this->_client[fd]->channel.size())
	{
		if (this->_client[fd]->channel[i++].name == channel)
		 	return (1);
	}
	return (0);
}



int	server::searchForid(const std::string& name)
{
	std::map<int, client*>::iterator it = this->_client.begin();
	while(it != this->_client.end())
	{
		if(it->second->_nickname == name)
			return(it->first);
		it++;
	}
	return (-1);
}

