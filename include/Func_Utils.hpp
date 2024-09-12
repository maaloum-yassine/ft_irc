/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Func_Utils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymaaloum <ymaaloum@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 04:03:00 by ymaaloum          #+#    #+#             */
/*   Updated: 2024/09/12 01:50:06 by ymaaloum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FUNC_UTILS_HPP
#define _FUNC_UTILS_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <cerrno>
#include <cstring>  // Pour strerror
#include <arpa/inet.h> // Pour inet_ntoa
#include <vector>
#include <map>
#include <poll.h>
#include <string>
#include <sstream>
#include <algorithm>
#include "channels.hpp"

std::vector<std::string> 	splitByCR(const std::string&);
std::vector<std::string>	splitBySpaces(const std::string &);
std::vector<std::string> 	split(const std::string&, char);
std :: string 				trim (const std :: string &);
std::string 				converUpper(std::string );
std::string					adminOrNot(bool);
int							count_words(const std::string& str, bool &tr);
int							checkCommand(std::string&);
bool						CheckDigit(const std:: string &);
bool						endOfCommand(const	std::string &);
void						printf_message(const std::string&, int);
int							checkChannelName(const std::string&);
int							convert(char *);
bool						limitNumber(const std::string&);
bool						check_arg(const char );





#endif
