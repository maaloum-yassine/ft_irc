/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 02:34:37 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/12 10:14:00 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP
#define _SERVER_HPP

#include "Parse.hpp"
#include "client.hpp"

typedef std::vector<pollfd>::iterator	pllfd_itertr;

enum EnumName
{
	INVITE_ONLY,
	TOPIC,
	KEY,
	LIMIT,
	OPERATOR,
	POSITIF
};
#define RPL_TOPICDISPLAY(hostname, nick, channel, topic) ":" + hostname + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_TOPICWHOTIME(topicsetter, nick, hostname, channelName) ":" + hostname + " 333 " + nick + " " + channelName + " " + topicsetter + "!~" + topicsetter + "@" + hostname + "\r\n"
#define RPL_NAMREPLY(hostname, clients, channelname, nick) ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
#define RPL_ENDOFNAMES(hostname, nick, channelname) ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"


#define RPL_DELOP(hostname, nick, channel, username, target) ":" + nick + "!~" + username + "@" + hostname + " MODE " + channel + " -o " + target + "\r\n"
#define ERR_NOTOP(hostname, channel) ":" + hostname + " 482 " + channel + " " + ":You're not a channel operator\r\n"
#define ERR_CHANNELISFULL(hostname, nick, channelName) ":" + hostname + nick + " 471 " + channelName + " :Cannot join channel (+l) - channel is full, try again later\r\n"
#define RPL_NICK_SET(hostname, new_nick) ":" + new_nick + "!" + new_nick + "@" + hostname + " NICK :" + new_nick + "\r\n"
#define ERR_UNKNOWNMODE(hostname, nick, c) ":" + hostname + " 472 " + nick + " " + c + " :is unknown mode char to me\r\n"
#define RPL_ADDMODE(hostname, nick, channel, mode, arg, username) ":" + nick + "!~" + username + "@" + hostname + " MODE " + channel + " " + mode + " " + arg + "\r\n"
#define RPL_DELMODE(hostname, nick, channel, mode, username) ":" + nick + "!~" + username + "@" + hostname + " MODE " + channel + " " + mode + " " + "\r\n"
#define RPL_INVITE(nick, username, clienthostname, invited, channel) ":" + nick + "!" + username + "@" + clienthostname + " INVITE " + invited + " :" + channel + "\r\n"
#define RPL_INVITING(hostname, nick, invited, chann) ":" + hostname + " 341 " + nick + " " + invited + " " + chann + "\r\n"
#define ERR_CHANOPRIVSNEEDED(hostname, nick, chann) ":" + hostname + " 482 " + nick + " " + chann + " :You're not channel operator\r\n"
#define ERR_USERNOTINCHANNEL(hostname, nick, nick2, chann) ":" + hostname + " 441 " + nick + " " + nick2 + " " + chann + " " + " :They aren't on that channel\r\n"
#define RPL_KICK(kicker, username, host, channel, targetuser, reason) ":" + kicker + "!" + username + "@" + host + " KICK " + channel + " " + targetuser + " :" + reason + "\r\n"
#define ERR_NOTONCHANNEL(hostname, chann) ":" + hostname + " 442 " + chann + " " + ":You're not on that channel\r\n"
#define RPL_TOPIC(hostname, nick, chann, topic) ":" + hostname + " 332 " + nick + " " + chann + " :" + topic + "\r\n"
#define ERR_NOSUCHNICK(hostname, nick) ":" + hostname + " 401 " + nick + " "  + " :No such nick\r\n"
#define ERR_CANNOTSENDTOCHAN(hostname, nick, channel) ":" + hostname + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n"
#define ERR_NORECIPIENT(hostname, nick ,cmd)  ":" + hostname  + " 411 " + nick + " :No recipient given (" + cmd + ")\r\n"
#define ERR_NOTEXTTOSEND(hostname, nick)  ":" + hostname  + " 412 " + nick + " :No text to send\r\n"
#define ERR_NOSUCHCHANNEL(hostname, nick, channel) ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_NEEDMOREPARAMS(nick, hostname, cmd) ":" + hostname + " 461 " + nick + " " + cmd + " :Not enough parameters\r\n"
#define PRIVMSG_FORMAT(senderNick, senderUsername, senderHostname, receiver, message) ":" + senderNick + "!~" + senderUsername + "@" + senderHostname + " PRIVMSG " + receiver + " :" + message + "\r\n"
#define RPL_JOIN(nick, username, channelname, ipaddress) ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
#define ERR_NOSUCHCHANNEL(hostname, nick, channel) ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_CHANOPRIVSNEEDED(hostname, nick, chann) ":" + hostname + " 482 " + nick + " " + chann + " :You're not channel operator\r\n"
#define ERR_USERONCHANNEL(hostname, nick, nick2, chann) ":" + hostname + " 443 " + nick + " " + nick2 + " " + chann + " :is already on channel\r\n"
#define ERR_NOTOP(hostname, channel) ":" + hostname + " 482 " + channel + " " + ":You're not a channel operator\r\n"
#define ERR_INVITEONLY(nick, hostname, channelName) ":" + hostname + " 473 " + nick + " " + channelName + " :Cannot join channel, you must be invited (+i)\r\n"
#define ERR_BADCHANNELKEY(nick, hostname, channelName) ":" + hostname + " 475 " + nick + " " + channelName + " :Cannot join channel (+K) - bad key\r\n"
#define RPL_QUIT(hostname, nick, quitMessage) ":" + hostname + "  QUIT " + nick + " :" + quitMessage + "\r\n"
#define RPL_COMMAND_NOT_FOUND(hostname, nick, command) ":" + hostname + " 404 " + nick + " " + command + " :Command not found\r\n"

class server
{
private:
	std::map<int, client*>       _client;            // Map pour stocker les clients connectés avec leur socket.
	std::vector<struct pollfd>   _fds;               // Vecteur pour les structures pollfd pour la gestion des événements.
	Parse                        _parse;             // Objet pour analyser les commandes des clients.
	const std::string            _port;              // Port sur lequel le serveur écoute.
	const std::string            _password;          // Mot de passe du serveur.
	int                          _serv_socket_fd;    // Descripteur de socket du serveur.

public:
	server(const std::string &, const std::string &);
	void display_err(const std::string &, bool) const;
	~server();

private:
	// Gestion du serveur
	// Fonctions pour démarrer le serveur, créer des sockets et gérer les connexions client.
	void start_server();
	void create_socket();
	void handle_cnx_client();
	void handle_msg_client(size_t &);
	// Gestion des commandes IRC
	// Fonctions pour appliquer et exécuter les commandes IRC.
	void commandApply(int, std::vector<std::string>&);
	void execute_cmd(int, const std::vector<std::string>&, int, const std::string&);
	// Gestion des utilisateurs
	// Fonctions pour gérer les utilisateurs, y compris les vérifications de surnoms, l'invitation et la gestion des mots de passe.
	bool alreadyUsedNickname(const std::string&);
	void invite(int, const std::vector<std::string>&);

	// Gestion des canaux
	// Fonctions pour gérer les canaux IRC, y compris l'adhésion, les modes, les topics et les expulsions.
	bool availableChannel(const std::string&);
	void join(const std::vector<std::string>&, int);
	void part(int, const std::vector<std::string>&);
	void kick(int, const std::vector<std::string>&, const std::string&);
	void kickUser(int, int, const std::string&, const std::string&);
	void kickclients(const std::string&, int);
	bool on_channel(const std::string&, int);
	bool operator_user(const std::string&, int);
	bool channelMember(const std::string&, int);
	int find_channel_id(const std::string&, int);
	char modeChannel(const std::string&);
	void mode(int fd, const std::vector<std::string>&);
	void updateMode(const std::string&, int, char, const std::string&);
	void brodcastMode(const std::string&, const std::string&, int, const std::vector<std::string>&);
	void applicateMode(char mode, std::string, int fd, char, std::vector<std::string>);

	// Gestion des messages
	// Fonctions pour envoyer des messages privés et diffuser des messages à plusieurs clients.
	void brodcast(const std::string&, const std::string&, int fd);
	void prive_msg(client*, const std::vector<std::string>&, const std::string&, int);

	// Fonctions utilitaires
	// Fonctions diverses pour gérer les détails des utilisateurs et des canaux.
	int searchForid(const std::string&);
	int idChannelfd(const std::string&, int&);
	std::string clientChannels(const std::string&);
	void displayTopic(std::string const&, std::string const&);
	std::string topicName(const std::string&);
	void topic(int, const std::vector<std::string>&, const std::string&);
	void updateChannelTopic(const std::string&, const std::string&);
	bool topicMode(const std::string&, int);
	char memberChannelNumbers(const std::string&);
	bool checkInvitedPersonnes(const std::string&, int, int);
	void updateclients(const std::string&, int fd);
	int idChannel(const std::string&, int&);
	std::string reason(const std::string&, int);
	void inserUser(const std::string&, const std::string&);
};

#endif // SERVER_HPP
	void nick(client*, const std::string&, int);
	void user(client*, const std::string&, int);
	void check_password(client*, const std::string&, const std::string&, int);
