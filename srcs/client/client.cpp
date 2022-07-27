/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/26 22:41:20 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(t_pollfd fds, Server *serverRef) : _mode(""), _clientStatus(PENDING), _clientType(TYPE_ZERO), _serverRef(serverRef), _fds(fds)
{
	/* connection registration */
	_messageFunctions["NICK"] = &Client::NICK;
	_messageFunctions["USER"] = &Client::USER;
	_messageFunctions["MODE"] = &Client::MODE;
	_messageFunctions["SERVICE"] = &Client::SERVICE;
	_messageFunctions["QUIT"] = &Client::QUIT;
	_messageFunctions["SQUIT"] = &Client::SQUIT;

	_messageFunctions["JOIN"] = &Client::JOIN;
	_messageFunctions["PART"] = &Client::PART;
	_messageFunctions["TOPIC"] = &Client::TOPIC;
	_messageFunctions["NAME"] = &Client::NAME;
	_messageFunctions["LIST"] = &Client::LIST;
	_messageFunctions["INVITE"] = &Client::INVITE;
	_messageFunctions["KICK"] = &Client::KICK;

	/* Sever Queries and Messages*/
	_messageFunctions["MOTD"] = &Client::MOTD;
	_messageFunctions["LUSERS"] = &Client::LUSERS;
	_messageFunctions["VERSION"] = &Client::VERSION;
	_messageFunctions["STATS"] = &Client::STATS;
	_messageFunctions["LINKS"] = &Client::LINKS;
	_messageFunctions["TIME"] = &Client::TIME;
	_messageFunctions["CONNECT"] = &Client::CONNECT;
	_messageFunctions["TRACE"] = &Client::TRACE;
	_messageFunctions["ADMIN"] = &Client::ADMIN;
	_messageFunctions["INFO"] = &Client::INFO;

	/* Miscellaneous Message */
	_messageFunctions["KILL"] = &Client::KILL;
	_messageFunctions["PING"] = &Client::PING;
	_messageFunctions["PONG"] = &Client::PONG;

	/* Service Queries and commands */
	_messageFunctions["SERVLIST"] = &Client::SERVLIST;
	_messageFunctions["SQUERY"] = &Client::SQUERY;


	_messageFunctions["CAP"] = &Client::SQUIT;

	_messageFunctions["PRIVMSG"] = &Client::PRIVMSG;

	_registered = false;
	_clientType = TYPE_ZERO;
	_nickname = "";
	_availableModes = "aiwroOs";
}

Client::~Client()
{
	return;
}

Client::Client(Client const &rhs) : _serverRef(rhs.getServer()), _fds(rhs.getPoll())
{
	this->_messageFunctions = rhs._messageFunctions;
	this->_nickname = rhs._nickname;
}

Client &Client::operator=(Client const &rhs)
{
	this->_fds = rhs._fds;
	this->_messageFunctions = rhs._messageFunctions;
	this->_nickname = rhs._nickname;
	return (*this);
}

bool Client::isDigit(char c) const
{
	return (c >= '0' && c <= '9');
}

bool Client::isLetter(char c) const
{
	return ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'));
}

bool Client::isSpecial(char c) const
{
	return ((c <= '}' && c >= '{') && (c >= '[' && c <= '`'));
}

std::string Client::getNickname() const
{
	return (this->_nickname);
}

std::string Client::getUsername() const
{
	return (this->_username);
}

struct pollfd Client::getPoll() const
{
	return (this->_fds);
}

Server *Client::getServer() const
{
	return (this->_serverRef);
}

Client::t_messFuncMap Client::getMessageFunctions() const
{
	return (this->_messageFunctions);
}

int Client::executeCommands()
{
	int ret;
	std::string errorStr;

	while (this->_commands.size() != 0)
	{
		//(this->*_messageFunctions.find((*_commands.begin())->getPrefix()) != this->*_messageFunctions->end())) &&
		// if the user is in a chat and there is no matchin command : do nothing
		try
		{
			if (_channels.size() == 0 && (ret = (this->*_messageFunctions.at((*_commands.begin())->getPrefix()))((**(_commands.begin())))) != 0)
			{
				errorStr = (*_commands.begin())->getErrorString(ret);
				send(this->getPoll().fd, errorStr.c_str(), errorStr.size(), 0);
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
		_commands.erase(_commands.begin());
	}
	return (0);
}

/**
 * @brief quick way to send a reply after a command
 * 
 * @param replyNum has to be one of the RPL_ defined in command.cpp
 * @return int 
 */
int Client::sendReply(int replyNum)
{
	std::string errorStr;

	errorStr = (*_commands.begin())->getErrorString(replyNum);
	send(this->getPoll().fd, errorStr.c_str(), errorStr.size(), 0);
	return (0);
}

Status Client::getStatus()
{
	return _clientStatus;
}

// std::map<std::string, int(Client*)(Command)>	Client::getFunction()
// {
// 	return _functionCmd;
// }

std::vector<Command *> Client::getCommands()
{
	return _commands;
}

void Client::setStatus(Status newStatus)
{
	_clientStatus = newStatus;
}

void Client::setPoll(t_pollfd newPoll)
{
	_fds = newPoll;
}

void Client::setNickname(std::string newNickname)
{
	_nickname = newNickname;
}

void Client::setChannel(Channel *channel)
{
	_channels.push_back(channel);
}

void Client::clearCommands()
{
	_commands.clear();
}

void Client::treatMessage()
{
	char buffer[BUFFER_SIZE + 1];
	std::string message;
	int ret;
	int i, start;

	memset(buffer, 0, BUFFER_SIZE);
	ret = recv(this->getPoll().fd, buffer, 1024, 0);

	if (ret == 0)
	{
		std::cout << "DISCONNECTED : " << std::endl;
		_clientStatus = DISCONNECTED;
		return;
	}
	buffer[ret] = 0;
	message += buffer;
	if (*(message.end() - 1) == '\n' && *(message.end() - 2) == '\r' && message.length() > 2)
	{
		i = 0;
		start = 0;
		while (message[i] && message[i + 1])
		{
			if (message[i] == '\r' && message[i + 1] == '\n')
			{
				// std::cout << i << message.substr(start, i - start) << std::endl;
				_commands.push_back(new Command(message.substr(start, i - start), getServer(), this));
				start = i + 2;
				i += 2;
			}
			else
				i++;
		}
		if (_clientStatus != CONNECTED)
		{
			std::cout << "PENDING : " << std::endl;

			if (_commands.size() > 2)
			{
				std::string nick = (*_commands[1]).getParameters()[0];
				std::map<int, Client *> clients = getServer()->getClients();
				for (std::map<int, Client *>::iterator cli = clients.begin(); cli != clients.end(); cli++)
				{
					if (cli->second->getNickname() == nick)
						nick += "_";
				}
				_nickname = (*_commands[1]).getParameters()[0];
				_username = (*_commands[2]).getParameters()[0];
				_clientStatus = PENDING;
			}
			else
			{
				std::cout << "REFUSED : " << std::endl;
				_clientStatus = REFUSED;
			}
		}
	}
	else if (_clientStatus != CONNECTED)
		_clientStatus = REFUSED;
}
