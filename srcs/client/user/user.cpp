/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 11:51:26 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/27 15:15:45 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "operator/operator.hpp"

User::User(Client const & rhs, std::string username, std::string realname) : Client(rhs), _username(username), _realname(realname)
{
	_commands = rhs.getCommands();
	_messageFunctions = rhs.getMessageFunctions();
	_mode = rhs.getMode();
	_clientStatus = rhs.getStatus();
	_clientType = rhs.getType();
}

std::string User::getUsername() const
{
	return (this->_username);
}

std::string User::getRealname() const
{
	return (this->_realname);
}

User::~User()
{

}

/* Command: OPER
   Parameters: <name> <password>

   A normal user uses the OPER command to obtain operator privileges.
   The combination of <name> and <password> are REQUIRED to gain
   Operator privileges.  Upon success, the user will receive a MODE
   message (see section 3.1.5) indicating the new user modes.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              RPL_YOUREOPER
           ERR_NOOPERHOST                  ERR_PASSWDMISMATCH

   Example:

   OPER foo bar                    ; Attempt to register as an operator
                                   using a username of "foo" and "bar"
                                   as the password.
 */
int User::OPER(Command arguments)
{
	if (arguments.getParameters().size() < 2)
		return (ERR_NEEDMOREPARAMS);
	if (_clientType == TYPE_OPERATOR)
		return (RPL_YOUREOPER);
	if (!(this->_serverRef->correctPassword(arguments.getParameters()[1])))
		return (ERR_PASSWDMISMATCH);
	_clientType = TYPE_OPERATOR;
	// TODO what about the username ?
	std::string name = arguments.getParameters()[0];
	this->_serverRef->changeClientClass(this, (new Operator(*this, name)));
	return (0);
}
