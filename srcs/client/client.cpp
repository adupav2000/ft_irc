/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/18 19:57:46 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "channelOperations.cpp"
#include "connectionRegistration.cpp"

Client::Client()
{
;
}

Client::Client(t_pollfd	fds) : _fds(fds) 
{
	/* connection registration */
	_messageFunctions["NICK"] = &NICK;
	_messageFunctions["USER"] = &USER;
	_messageFunctions["MODE"] = &MODE;
	_messageFunctions["SERVICE"] = &SERVICE;
	_messageFunctions["QUIT"] = &QUIT;
	_messageFunctions["SQUIT"] = &SQUIT;
}

Client::~Client()
{

}

Client::Client(Client const & rhs)
{

}

Client::~Client()
{

}

Client &Client::operator=(Client const & rhs)
{

}

int Client::isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

int Client::isLetter(char c)
{
	return ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'));
}

int Client::isSpecial(char c)
{
	return ((c <= '}' && c >= '{') && (c >= '[' && c <= '`'));
}

