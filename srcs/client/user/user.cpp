/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 11:51:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/26 13:07:43 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"

User::User(Client const & rhs) : Client(rhs)
{
	_serverRef = rhs.getServer();
	_fds = rhs.getPoll();
	this->_messageFunctions = rhs.getMessageFunctions();
	this->_nickname = rhs.getNickname();
}

User::~User()
{

}
