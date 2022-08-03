/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamanfo <kamanfo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/08/03 23:12:25 by kamanfo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(t_pollfd fds, Server *serverRef) : _mode(""), _clientStatus(NEW), _clientType(TYPE_ZERO), _serverRef(serverRef), _fds(fds), _passOK(false)
{
	/* connection registration */
	_messageFunctions["PASS"] = &Client::PASS;
	_messageFunctions["NICK"] = &Client::NICK;
	_messageFunctions["USER"] = &Client::USER;
	// UNSAFE
	_messageFunctions["USERHOST"] = &Client::USER;
	_messageFunctions["MODE"] = &Client::MODE;
	_messageFunctions["OPER"] = &Client::OPER;
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

	/* userBasedQueries */
	_messageFunctions["WHO"] = &Client::WHO;
	_messageFunctions["WHOIS"] = &Client::WHOIS;
	// _messageFunctions["WHOWAS"] = &Client::WHOWAS;

	_registered = false;
	_clientType = TYPE_ZERO;
	_nickname = "";
	_availableModes = "aiwroOs";
}

Client::~Client()
{
	for (size_t i = 0; i < _commands.size(); i++)
		delete _commands[i];
	_commands.clear();
	std::cout << "clients : " << std::endl;
	return;
}

Client::Client(Client const &rhs) : _serverRef(rhs.getServer()), _fds(rhs.getPoll())
{
	this->_messageFunctions = rhs._messageFunctions;
	this->_nickname = rhs._nickname;
	this->_commands = rhs.getCommands();
}

Client &Client::operator=(Client const &rhs)
{
	_commands = std::vector<Command *>(rhs.getCommands());
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
	return ((c <= '}' && c >= '{')
		|| (c >= '[' && c <= '`')
		|| (c <= '@' && c >= '!'));
}

/* Getters */

std::string Client::getNickname() const
{
	return (this->_nickname);
}

std::string Client::getUsername() const
{
	return (this->_username);
}

std::string Client::getMode() const
{
	return (this->_mode);
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

/* Command execution */

int Client::executeCommands()
{
	int ret;
	std::string errorStr;
	unsigned long i = 0;
	std::vector<Command *>::iterator it;

	while (this->_commands.size() != 0 && _clientStatus != REFUSED)
	{
		//(this->*_messageFunctions.find((*_commands.begin())->getPrefix()) != this->*_messageFunctions->end())) &&
		// if the user is in a chat and there is no matchin command : do nothing
		std::cout << (*_commands.begin())->getPrefix();
		i = 0;
		while (i < (*_commands.begin())->getParameters().size())
			std::cout << " " << (*_commands.begin())->getParameters()[i++];
		std::cout << std::endl;
		try
		{
			// std::cout << "Talking from the executeCommand() : " << (*_commands.begin())->getPrefix() << std::endl;
			if (this->_messageFunctions.find((*_commands.begin())->getPrefix()) == this->_messageFunctions.end())
			{
				_commands.erase(_commands.begin());
				continue;
			}
			ret = (this->*_messageFunctions.at((*_commands.begin())->getPrefix()))((**(_commands.begin())));
			// std::cout << "Result function " << ret << std::endl;
			// si pas de channel et le premier n'est pas une commande
			if (ret == -4)
				return (-1);
			if (this->_messageFunctions.find((*_commands.begin())->getPrefix()) != this->_messageFunctions.end() && ret != 0)
			{
				errorStr = this->_nickname + "!" + this->_username + "@" + this->_hostname + " " + (*_commands.begin())->getStringCommand();
				send(this->getPoll().fd, errorStr.c_str(), errorStr.size(), 0);
				errorStr = (*_commands.begin())->getErrorString(ret);
				send(_fds.fd, errorStr.c_str(), errorStr.size(), 0);
			}
			if (_clientStatus == NEW)
			{
				if ((*_commands.begin())->getPrefix() != "CAP" && (*_commands.begin())->getPrefix() != "PASS" && (*_commands.begin())->getPrefix() != "USER" && (*_commands.begin())->getPrefix() != "NICK")
				{
					_clientStatus = REFUSED;
					return 0;
				}
				if ((_serverRef->getPassword() != "" && !_passOK) && (getUsername().size() != 0 || getNickname().size() != 0))
				{
					_clientStatus = REFUSED;
					return 0;
				}
			}
			it = _commands.erase(_commands.begin());
			//delete (*it);
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	if (getStatus() != PENDING)
		_commands.clear();
	return (0);
}

/**
 * @brief quick way to send a reply after a command
 *
 * @param replyNum has to be one of the RPL_ defined in command.cpp
 * @return int
 */
int Client::sendReply(std::vector<int> replyNum)
{
	std::string errorStr;
	
	// std::cout << errorStr << std::endl;
	// sending the introduction
	errorStr = this->_nickname + "!" + this->_username + "@" + this->_hostname + " " + (*_commands.begin())->getStringCommand() + "\r\n";
	send(this->getPoll().fd, errorStr.c_str(), errorStr.size(), 0);
	for (std::vector<int>::iterator it = replyNum.begin(); it != replyNum.end(); it++)
	{
		errorStr = (*_commands.begin())->getErrorString(*it);
		send(this->getPoll().fd, errorStr.c_str(), errorStr.size(), 0);
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
	
	if (replyNum <= ERR_NOSUCHNICK && replyNum >= ERR_USERSDONTMATCH)
	{
	errorStr = this->_nickname + "!" + this->_username + "@" + this->_hostname + " " + (*_commands.begin())->getStringCommand() + "\r\n";
	send(this->getPoll().fd, errorStr.c_str(), errorStr.size(), 0);
	}
	errorStr = ":" + getNickname() + "!" + getUsername() + "@" + this->_hostname + " " + (*_commands.begin())->getErrorString(replyNum);
	std::cout << "sending reply for " << errorStr << std::endl;
	send(this->getPoll().fd, errorStr.c_str(), errorStr.size(), 0);
	return (0);
}

Status Client::getStatus() const
{
	return _clientStatus;
}

Type Client::getType() const
{
	return _clientType;
}

// std::map<std::string, int(Client*)(Command)>	Client::getFunction()
// {
// 	return _functionCmd;
// }

std::vector<Command *> Client::getCommands() const
{
	return _commands;
}

std::vector<Channel *> Client::getChannels() const
{
	return _channels;
}

std::string Client::getHostname() const
{
	return _hostname;
}

std::string Client::getRealname() const
{
	return _realname;
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
		_clientStatus = DISCONNECTED;
		_text.clear();
		return;
	}
	buffer[ret] = 0;
	_text += buffer;
	if (*(_text.end() - 1) == '\n' && *(_text.end() - 2) == '\r' && _text.length() > 2)
	{
		i = 0;
		start = 0;
		while (_text[i] && _text[i + 1])
		{
			if (_text[i] == '\r' && _text[i + 1] == '\n')
			{
				_commands.push_back(new Command(_text.substr(start, i - start), getServer(), this));
				start = i + 2;
				i += 2;
			}
			else
				i++;
		}
		if (_clientStatus != CONNECTED)
		{

			if (_commands.size() > 2)
			{
				if (_serverRef->getPassword() != "" && _commands[1]->getPrefix() != "PASS")
					_clientStatus = REFUSED;
				else
				{
					_clientStatus = PENDING;
					executeCommands();
				}
				//_nickname = nick;
				_username = (*_commands[2]).getParameters()[0];
				_clientStatus = PENDING;
			}
			else
			{
				_clientStatus = REFUSED;
			}
		}
		_text.clear();
	}
	else if (*(_text.end() - 1) == '\n' && (_clientStatus == NEW || _clientStatus == CONNECTED))
	{
		if (_text.size() > 2)
		{
			*(_text.end() - 1) = '\r';
			_text += "\n";
			_commands.push_back(new Command(_text.substr(0, _text.size() - 2), getServer(), this));
			if (_clientStatus != CONNECTED)
			{
				executeCommands();
				if (getUsername().size() != 0 && getNickname().size() != 0)
				{
					if (_passOK == true || _serverRef->getPassword().size() == 0)
						_clientStatus = PENDING;
					else
					{
						_username = "";
						_nickname = "";
						_clientStatus = REFUSED;
					}
				}
			}
		}
		_text.clear();
	}
	else if (_clientStatus != CONNECTED)
		_clientStatus = REFUSED;
}

void Client::leaveChannel(Channel *channel)
{
	for (std::vector<Channel *>::iterator it = _channels.begin(); it < _channels.end(); it++)
	{
		if (*it == channel)
		{
			//delete channel;
			_channels.erase(it);
		}
	}
}
