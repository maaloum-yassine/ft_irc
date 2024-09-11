/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:37:50 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/11 09:54:25 by ymaaloum         ###   ########.fr       */
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

	void	nick(client* _client ,const std::string & _nicknm, int fd)
	{
		_client->addNickname(_nicknm);
		printf_message(RPL_NICK_SET(_client->_ipclient, _nicknm), fd);
		_client->_nicknameB = 1;
		if(_client->_passB && _client->_nicknameB && _client->_usernameB)
			printf_message( ":ircserv_KAI->chat 001 " + _client->_nickname + " :Welcome to the IRC Network, " + _client->_nickname + " \r\n", fd);
	}
	/********************************************************************************/


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


	/*********************************************************************************/

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



	/*********************************************************************************/


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

	/*********************************************************************************/

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
								// message(RPL_NAMREPLY(this->_client[fd].ipclient, clientChannels(channelSplited[i])  , channelSplited[i], clientServer[fd].nickname) , fd);
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

	/*********************************************************************************/
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
	/**********************************************************************************/
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



	/************************************************************************************/
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


	/******************************************************************************* */

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

	/*********************************************************************************/
	void	server::execute_cmd(int fd,const std :: vector<std::string>& split_cmd , int index_cmd, const std::string & commandLine)
	{
		switch (index_cmd)
		{
			case 1:
				if (split_cmd.size() > 1)
					check_password(this->_client[fd], split_cmd[1], this->_password, fd);
				break;
			case 2:
				if (!alreadyUsedNickname(split_cmd[1]))
					nick(this->_client[fd], split_cmd[1], fd);
				else
				{
					this->_client[fd]->_duplicateNickname = 1;
					printf_message("Error :Duplicated nickname \n\r", fd);
				}
				break;
			case 3 :
					user(this->_client[fd], commandLine ,fd);
					break ;
			case 4  :
					prive_msg(this->_client[fd], split_cmd, commandLine, fd);
					break ;
			case 5 :
					if (this->_client[fd]->_connected)
						join(split_cmd, fd);
					break ;
			case 6:
					if (this->_client[fd]->_connected)
						topic(fd, split_cmd, commandLine);
					break ;
			case 7:
					if (this->_client[fd]->_connected)
						kick(fd, split_cmd, commandLine);
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



/*****************************************************/
server :: ~server()
{
	for (std::map<int, client*>::iterator it = _client.begin(); it != _client.end(); ++it)
		delete it->second;
	this->_client.clear();
}

