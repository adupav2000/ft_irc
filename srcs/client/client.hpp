/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:29 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/26 12:45:06 by AlainduPa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_HPP
#define Client_HPP
#include <string>
#include <iostream>
#include <map>
#include <sys/stat.h>
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

enum Type{
	TYPE_ZERO,
	TYPE_PASS,
	TYPE_CLIENT,
	TYPE_SERVICE,
	TYPE_USER,
	TYPE_OPERATOR
};

class Client
{
public:
    Client(struct pollfd _fds, Server *serverRef);
    Client(Client const & rhs);
    ~Client();
    Client &operator=(Client const & rhs);

	typedef struct pollfd	t_pollfd;
	typedef std::map<std::string, int (Client::*)(Command)> t_messFuncMap;
	
    /* handling messages and commands */
	int		execCommand(std::string arguments);
    void	treatMessage();
	int		executeCommands();

	/* Getters */
	std::string				getNickname() const;
	std::string     		getUsername() const;
	struct pollfd			getPoll() const;
	Server		        	*getServer() const;
	Client::t_messFuncMap	getMessageFunctions() const;
	Status 					getStatus();
	std::vector<Command *>  getCommands();

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
	typedef std::map<std::string, int (Client::*)(Command)> t_messFuncMap;
	bool 					_registered;
	std::vector<Command *>	_commands;
	std::vector<Channel *> _channels;
	t_messFuncMap	_messageFunctions;
	std::string		_nickname;
	std::string		_username;
	std::string		_mode;
	Status 			_clientStatus;
	Type			_clientType;

	std::string _availableModes;

	/* Server side variables */
	Server					*_serverRef;
	t_pollfd				_fds;
	std::string 	_text;/* used to store text */

	/* Connection registration functions*/
	int PASS(Command);
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
	int TOPIC(Command);
	int NAME(Command);
	int LIST(Command);
	int INVITE(Command);
	int KICK(Command);

	int	MOTD(Command);
	int LUSERS(Command);
	
	/* Channel function */
	int modeChannel(Command arguments);

	/* Utils */
	bool isDigit(char c) const;
	bool isLetter(char c) const;
	bool isSpecial(char c) const;
	int	checkNickname(Command *, std::string) const;

};

#endif
