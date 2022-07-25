/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/25 18:01:13 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(t_pollfd	fds, Server &serverRef) : _mode(""),  _clientStatus(PENDING), _clientType(TYPE_ZERO), _serverRef(serverRef), _fds(fds)
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

	_messageFunctions["CAP"] = &Client::SQUIT;

	_registered = false;
	_clientType = TYPE_ZERO;
	_nickname = "";
	_availableModes = "aiwroOs";
}

Client::~Client()
{
	return ;
}


Client::Client(Client const & rhs) : _serverRef(rhs.getServerRef()), _fds(rhs.getPoll())
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
struct pollfd	Client::getPoll() const
{
	return (this->_fds);
}

Server			&Client::getServerRef() const
{
	return (this->_serverRef);
}

Client::t_messFuncMap			Client::getMessageFunctions() const
{
	return (this->_messageFunctions);
}

int Client::executeCommands()
{
	int			ret;
	std::string errorStr;

	while (this->_commands.size() != 0)
	{
		//(this->*_messageFunctions.find((*_commands.begin())->getPrefix()) != this->*_messageFunctions->end())) &&
		if ((ret = (this->*_messageFunctions.at((*_commands.begin())->getPrefix()))((**(_commands.begin())))) != 0)
		{
			errorStr = (*_commands.begin())->getErrorString(ret);
			send(this->getPoll().fd, errorStr.c_str(), errorStr.size(), 0);
		}
		else
		{
			//TODO ? CHANGE
			std::cout << "Command: " << (*_commands.begin())->getPrefix() << " failed" <<  std::endl;
		}
		_commands.erase(_commands.begin());
	}
	return (0);
}

Status 			Client::getStatus()
{
	return _clientStatus;
}

void 			Client::setStatus(Status newStatus)
{
	_clientStatus = newStatus;
}

void 			Client::setPoll(t_pollfd newPoll)
{
	_fds = newPoll;
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
				_commands.push_back(new Command(message.substr(start, i - start)));
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
				_clientStatus = PENDING;
			}
			else
				_clientStatus = REFUSED;
			}
	}
	else
		if (_clientStatus != CONNECTED)
			_clientStatus = REFUSED;
   this->executeCommands();
}