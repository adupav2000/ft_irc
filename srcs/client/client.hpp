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
class Command;
class Channel;

enum Status {
	CONNECTED,
	DISCONNECTED,
	PENDING,
	REFUSED
};

class Client
{
public:
    Client(struct pollfd _fds, Server *serverRef);
    Client(Client const & rhs);
    ~Client();
    Client &operator=(Client const & rhs);

	typedef struct pollfd	t_pollfd;
	typedef std::map<std::string, int (Client::*)(std::string)> t_messFuncMap;


	int execCommand(std::string arguments);
	
    /* handling messages and commands */
    void	treatMessage();
	int		executeCommands();

	/* Getters */
	std::string		getNickname() const;
	std::string		getUsername() const;
	struct pollfd	getPoll() const;
	Server			*getServer() const;
	Status 			getStatus();
	
	// std::map<std::string, int(Client*)(Command)>	getFunction();
	std::vector<Command *> getCommands();

	/* Setters */
	void 			setStatus(Status newStatus);
	void 			setPoll(t_pollfd newPoll);
	void			setNickname(std::string newNickname);
	void			setChannel(Channel *channel);

	void clearCommands();


protected:
	/* Variables */
	bool 	_registered;
	std::vector<Command *> _commands;
	//std::map<std::string, int(Client::*)(Command)> _functionCmd;
	std::vector<Channel *> _channels;
	typedef std::map<std::string, int (Client::*)(Command)> t_messFuncMap;
	t_messFuncMap	_messageFunctions;
	std::string		_nickname;
	std::string		_username;
	std::string		_mode;
	Status 			_clientStatus;

	/* Server side variables */
	Server					*_serverRef;
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
