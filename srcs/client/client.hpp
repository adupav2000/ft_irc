/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:29 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/22 17:47:19 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_HPP
#define Client_HPP
#include <string>
#include <iostream>
#include <map>
#include <poll.h>
#include <utility>

#include "../server/server.hpp"
#include "../globals.hpp"
#include "../command/command.hpp"
#define BUFFER_SIZE 1024

class Server;

class Client
{
public:
    Client(struct pollfd _fds, Server &serverRef);
    Client(Client const & rhs);
    ~Client();
    Client &operator=(Client const & rhs);
	int execCommand(std::string arguments);
	
    /* handling messages and commands */
    void	treatMessage();
	int		executeCommands();

	/* Getters */
	std::string		getNickname() const;
	struct pollfd	getPoll() const;
	Server			&getServerRef() const;

protected:
	/* Variables */
	bool 	_registered;
	std::vector<Command *> _commands;
	std::map<std::string, void(*)(Command *)> functionCmd;
	typedef std::map<std::string, int (Client::*)(Command)> t_messFuncMap;
	t_messFuncMap	_messageFunctions;
	std::string		_nickname;
	std::string		_mode;

	/* Server side variables */
	Server					&_serverRef;
	typedef struct pollfd	t_pollfd;
	t_pollfd				_fds;

	/* Connection registration functions*/
	int NICK(Command);
	int USER(Command);
	int OPER(Command);
	int MODE(Command);
	int SERVICE(Command);
	int QUIT(Command);
	int SQUIT(Command);

	/* Channel operations */
	int JOIN(Command);
	int PART(Command);

	/* Utils */
	bool isDigit(char c) const;
	bool isLetter(char c) const;
	bool isSpecial(char c) const;

};

#endif
