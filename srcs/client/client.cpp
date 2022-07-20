/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/18 15:10:29 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client()
{
	return ;
	// _messageFunctions.insert(std::pair<std::string, int (*)(std::string)>("NICK", &NICK));
}

Client::~Client()
{
	return ;
}

Client::Client(struct pollfd poll) : _fds(poll)
{

}

// Client::Client(Client const & rhs)
// {

// }

// Client &Client::operator=(client const & rhs)
// {

// }

struct pollfd const &Client::getPoll()
{
	return this->_fds;
}

std::string const &Client::getNickname()
{
	return this->_nickname;
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

void Client::treatMessage()
{
	char buffer[BUFFER_SIZE + 1];
	std::string message;
	int ret;
	int i, start;	

	memset(buffer, 0, BUFFER_SIZE);
	ret = recv(this->getPoll().fd, buffer, 1024, 0);
	if (ret == 0)
		return;
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
	}

}
