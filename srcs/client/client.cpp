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

client::client()
{
	_messageFunctions.insert(std::pair<std::string, int (*)(std::string)>("NICK", &NICK));
}

client::~client()
{

}

client::client(client const & rhs)
{

}

client::~client()
{

}

client &client::operator=(client const & rhs)
{

}

int client::isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

int client::isLetter(char c)
{
	return ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'));
}

int client::isSpecial(char c)
{
	return ((c <= '}' && c >= '{') && (c >= '[' && c <= '`'));
}

