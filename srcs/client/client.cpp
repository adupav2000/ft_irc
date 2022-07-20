/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/20 17:23:43 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(t_pollfd	fds, Server &serverRef) : _serverRef(serverRef), _fds(fds)  
{
	/* connection registration */
	_messageFunctions["NICK"] = &Client::NICK;
	_messageFunctions["USER"] = &Client::USER;
	_messageFunctions["MODE"] = &Client::MODE;
	_messageFunctions["SERVICE"] = &Client::SERVICE;
	_messageFunctions["QUIT"] = &Client::QUIT;
	_messageFunctions["SQUIT"] = &Client::SQUIT;
}

Client::~Client()
{

}

Client::Client(Client const & rhs) : _serverRef(rhs.getServerRef()), _fds(rhs.getPollFds())
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

int Client::isDigit(char c) const
{
	return (c >= '0' && c <= '9');
}

int Client::isLetter(char c) const
{
	return ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'));
}

int Client::isSpecial(char c) const
{
	return ((c <= '}' && c >= '{') && (c >= '[' && c <= '`'));
}

std::string		Client::getNickname() const
{
	return (this->_nickname);
}
struct pollfd	Client::getPollFds() const
{
	return (this->_fds);
}

Server			&Client::getServerRef() const
{
	return (this->_serverRef);
}
