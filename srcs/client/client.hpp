/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:29 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/08/02 21:02:16 by adu-pavi         ###   ########.fr       */
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
	NEW,
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
	std::string     		getHostname() const;
	std::string     		getRealname() const;
	std::string     		getMode() const;
	struct pollfd			getPoll() const;
	Server		        	*getServer() const;
	Client::t_messFuncMap	getMessageFunctions() const;
	Status 					getStatus() const;
	Type 					getType() const;
	std::vector<Command *>  getCommands() const;
	std::vector<Channel *>	getChannels() const;


	
	/* Setters */
	void 			setStatus(Status newStatus);
	void 			setPoll(t_pollfd newPoll);
	void			setNickname(std::string newNickname);
	void			setChannel(Channel *channel);

	/* modifiers */
	void leaveChannel(Channel *channel);

	void clearCommands();

protected:
	/* Variables */
	bool 					_registered;
	std::vector<Command *>	_commands;
	std::vector<Channel *>	_channels;
	t_messFuncMap			_messageFunctions;
	std::string				_nickname;
	std::string				_username;
	std::string				_hostname;
	std::string				_realname;
	std::string				_mode;
	Status 					_clientStatus;
	Type					_clientType;
	std::string		_availableModes;
	/* Server side variables */
	Server					*_serverRef;
	t_pollfd				_fds;
	std::string 	_text;
	bool					_passOK;

	


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

	/* Sever Queries and Messages*/
	int	MOTD(Command);
	int LUSERS(Command);
	int VERSION(Command);
	int STATS(Command);
	int LINKS(Command);
	int TIME(Command);
	int CONNECT(Command);
	int TRACE(Command);
	int ADMIN(Command);
	int INFO(Command);

	/* Miscellaneous Message */
	int KILL(Command);
	int PING(Command);
	int PONG(Command);

	/* Service Queries and commands */
	int SERVLIST(Command);
	int SQUERY(Command);


	int PRIVMSG(Command);
	int NOTICE(Command);

	int WHO(Command);
	int WHOIS(Command);
	int WHOWAS(Command);

	/* Channel function */
	int modeChannel(Command arguments);

	/* Utils */
	bool isDigit(char c) const;
	bool isLetter(char c) const;
	bool isSpecial(char c) const;
	int	checkNickname(Command, std::string) const;
	int sendReply(std::vector<int> replyNum);
	int sendReply(int replyNum);
	int setUserMode(unsigned char num);
};

#endif
