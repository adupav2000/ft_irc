/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:29 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/18 19:56:36 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_HPP
#define Client_HPP
#include <string>
#include <iostream>
#include <map>
#include <utility>

#include "../server/server.hpp"
#include "../globals.hpp"

class Client
{
public:
    Client();
    Client(struct pollfd _fds, Server &serverRef);
    Client(Client const & rhs);
    ~Client();
    Client &operator=(Client const & rhs);
	Server &_serverRef;
	int execCommand(std::string arguments);
	
	/* Getters */
	std::string		getNickname();
	struct pollfd	getPollFds();

protected:
	/* Server side variables */
	typedef struct pollfd t_pollfd;
	t_pollfd _fds;

	/* Variables */
	typedef std::map<std::string, int (Client::*)(std::string)> t_messFuncMap;
	t_messFuncMap	_messageFunctions;
	std::string		_nickname;

	/* Connection registration functions*/
	int NICK(std::string);
	int USER(std::string);
	int MODE(std::string);
	int SERVICE(std::string);
	int QUIT(std::string);
	int SQUIT(std::string);

	/* Channel operations */
	int JOIN(std::string arguments);
	int PART(std::string arguments);

	/* Utils */
	int isDigit(char c);
	int isLetter(char c);
	int isSpecial(char c);

};

#endif