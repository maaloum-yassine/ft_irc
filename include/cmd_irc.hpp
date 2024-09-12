/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_irc.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 05:37:47 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/12 07:18:39 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_IRC_HPP
#define CMD_IRC_HPP

#include "server.hpp"

// class CmdIrc {
// public:
    // Vérification et gestion des pseudos
    bool alreadyUsedNickname(const std::string& nickname);
    void nick(client* _client, const std::string &nickname, int fd);

    // Gestion des canaux
    bool availableChannel(const std::string &channel);
    void join(const std::vector<std::string>& args, int fd);
    void part(int fd, const std::vector<std::string>& args);
    void kick(int fd, const std::vector<std::string>& args, const std::string &channel);
    void kickUser(int fd, int userId, const std::string &reason, const std::string &channel);
    bool channelMember(const std::string& channel, int fd);
    int idChannel(const std::string& channel, int &fd);
    int idChannelfd(const std::string& channel, int &fd);
    char memberChannelNumbers(const std::string& channel);
    bool checkInvitedPersonnes(const std::string &channel, int fd, int userId);
    void updateclients(const std::string &channel, int fd);
    void kickclients(const std::string& channel, int fd);

    // Gestion des messages
    void prive_msg(client* sender, const std::vector<std::string>& targets, const std::string &message, int fd);
    void brodcast(const std::string &message, const std::string &channel, int fd);
    void brodcastMode(const std::string &message, const std::string &channel, int fd, const std::vector<std::string>& args);

    // Gestion des sujets
    std::string topicName(const std::string& channel);
    void displayTopic(const std::string &channel, const std::string &topic);
    void topic(int fd, const std::vector<std::string>& args, const std::string &channel);
    void updateChannelTopic(const std::string &channel, const std::string &topic);
    bool topicMode(const std::string& channel, int fd);

    // Gestion des modes
    void mode(int fd, const std::vector<std::string>& args);
    void updateMode(const std::string &channel, int fd, char mode, const std::string &arg);
    void brodcastMode(const std::string &message, const std::string &channel, int fd, const std::vector<std::string>& args);
    void applicateMode(char mode, std::string channel, int fd, char flag, std::vector<std::string> args);

    // Utilisateurs
    void inserUser(const std::string& nickname, const std::string &password);
    void user(client* _client, const std::string& user_info, int fd);
    void check_password(client *_client, const std::string &pass, const std::string &pass_serv, int fd);
    bool operator_user(const std::string &nickname, int fd);

    // Divers
    std::string clientChannels(const std::string& nickname);
    int find_channel_id(const std::string& channel, int userId);
    int searchForid(const std::string& id);
    bool on_channel(const std::string& channel, int fd);
    std::string reason(const std::string& channel, int fd);
    void invite(int fd, const std::vector<std::string>& args);

// private:
//     server* _server; // Pointer to a server instance or another relevant member if needed
// };

#endif // CMD_IRC_HPP
