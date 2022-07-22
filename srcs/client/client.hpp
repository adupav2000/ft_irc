/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:29 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/20 21:10:15 by AlainduPa        ###   ########.fr       */
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

enum Status {
	CONNECTED,
	DISCONNECTED,
	PENDING,
	REFUSED
};

class Client
{
public:
    Client(struct pollfd _fds, Server &serverRef);
    Client(Client const & rhs);
    ~Client();
    Client &operator=(Client const & rhs);

	typedef struct pollfd	t_pollfd;

	int execCommand(std::string arguments);
	
    /* handling messages and commands */
    void treatMessage();

	/* Getters */
	std::string		getNickname() const;
	struct pollfd	getPoll() const;
	Server			&getServerRef() const;
	Status 			getStatus();

	/* Setters */
	void 			setStatus(Status newStatus);
	void 			setPoll(t_pollfd newPoll);

	void clearCommands();


protected:
	/* Variables */
	std::vector<Command *> _commands;
	std::map<std::string, void(*)(Command *)> functionCmd;
	typedef std::map<std::string, int (Client::*)(std::string)> t_messFuncMap;
	t_messFuncMap	_messageFunctions;
	std::string		_nickname;
	Status 			_clientStatus;

	/* Server side variables */
	Server					&_serverRef;
	t_pollfd				_fds;

	/* Connection registration functions*/
	int NICK(std::string);
	int USER(std::string);
	int MODE(std::string);
	int SERVICE(std::string);
	int QUIT(std::string);
	int SQUIT(std::string);

	/* Channel operations */
	int JOIN(std::string);
	int PART(std::string);

	/* Utils */
	int isDigit(char c) const;
	int isLetter(char c) const;
	int isSpecial(char c) const;

};

#endif
