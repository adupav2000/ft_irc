/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/22 17:26:57 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(t_pollfd	fds, Server *serverRef) : _mode(""), _clientStatus(PENDING), _serverRef(serverRef), _fds(fds) 
{
	/* connection registration */
	_functionCmd["NICK"] = &Client::NICK;
	_functionCmd["USER"] = &Client::USER;
	_functionCmd["MODE"] = &Client::MODE;
	_functionCmd["SERVICE"] = &Client::SERVICE;
	_functionCmd["QUIT"] = &Client::QUIT;
	_functionCmd["SQUIT"] = &Client::SQUIT;

	_registered = false;
}

Client::~Client()
{
	return ;
}


Client::Client(Client const & rhs) : _serverRef(rhs.getServer()), _fds(rhs.getPoll())
{
	this->_messageFunctions = rhs._messageFunctions;
	this->_nickname = rhs._nickname;
}


Client &Client::operator=(Client const & rhs)
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

std::string		Client::getNickname() const
{
	return (this->_nickname);
}

std::string		Client::getUsername() const
{
	return (this->_username);
}

struct pollfd	Client::getPoll() const
{
	return (this->_fds);
}

Server			*Client::getServer() const
{
	return (this->_serverRef);
}


int Client::executeCommands()
{
	// while (this->_commands.size() != 0)
	// {
	// 	this->_messageFunctions[_commands.begin()->getPrefix()](_commands.begin());
	// }
	return 0;
}

Status 			Client::getStatus()
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

void 			Client::setStatus(Status newStatus)
{
	_clientStatus = newStatus;
}

void 			Client::setPoll(t_pollfd newPoll)
{
	_fds = newPoll;
}

void			Client::setNickname(std::string newNickname)
{
	_nickname = newNickname;
}

void			Client::setChannel(Channel *channel)
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
		return;
	}
	buffer[ret] = 0;
	message += buffer;
	if (*(message.end() - 1) == '\n' && *(message.end() - 2) == '\r' && message.length() > 2)
	{
		i = 0;
		start = 0;
		while (message[i] && message[i+1])
		{
			if (message[i] ==  '\r' && message[i + 1] == '\n')
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
			if (_commands.size() > 0)
			{
				_nickname = (*_commands[1]).getParameters()[0];
				_username = (*_commands[2]).getParameters()[0];
				_clientStatus = PENDING;
			}
			else
				_clientStatus = REFUSED;
			}
	}
	else
		if (_clientStatus != CONNECTED)
			_clientStatus = REFUSED;
   executeCommands();
}