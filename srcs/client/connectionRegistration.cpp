/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionRegistration.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 11:58:54 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/27 14:19:35 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "user/user.hpp"
#include "user/operator/operator.hpp"
#include "../server/server.hpp"
#include "../globals.hpp"

class User;
/*
Command: PASS
   Parameters: <password>

   The PASS command is used to set a 'connection password'.  The
   optional password can and MUST be set before any attempt to register
   the connection is made.  Currently this requires that user send a
   PASS command before sending the NICK/USER combination.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

   Example:

           PASS secretpasswordhere
*/
int Client::PASS(Command arguments)
{
	if (arguments.getParameters().size() < 1)
		return (ERR_NEEDMOREPARAMS);
	if (_clientType > TYPE_ZERO)
		return (ERR_ALREADYREGISTRED);
	// TODO que se pass t'il si jamais le mots de passe est erroné ?
	if (this->getServer()->correctPassword(arguments.getParameters()[0]))
		return (0);
	_clientType = TYPE_PASS;
	return (0);
}

/*
   RFC 2812
   Command: NICK
   Parameters: <nickname>

   NICK command is used to give user a nickname or change the existing
   one.

   Numeric Replies:

        [X]   ERR_NONICKNAMEGIVEN             [X] ERR_ERRONEUSNICKNAME
        [X]   ERR_NICKNAMEINUSE                [] ERR_NICKCOLLISION
        []   ERR_UNAVAILRESOURCE              [X] ERR_RESTRICTED
*/
int Client::NICK(Command arguments)
{
	if (arguments.getParameters().size() < 1)
		return (ERR_NONICKNAMEGIVEN);
	if (_serverRef->nickNameUsed(arguments.getParameters()[0]))
		return (ERR_NICKNAMEINUSE);	
	if (_mode.find('r') != std::string::npos)
		return (ERR_RESTRICTED);
	std::string nTmp = arguments.getParameters()[0];
	/* Si jamais il y a plus de 2 arguments, cela veux dire qu'il 
	y a un espace */
	int retValNickname = this->checkNickname(arguments, nTmp);
	if (retValNickname != 0)
		return (retValNickname);
	_clientType = TYPE_CLIENT;
	this->_nickname = arguments.getParameters()[0];
	return (SEND_CONFIRMNEWNICK);
}

int	Client::checkNickname(Command arguments, std::string name) const
{
	if (arguments.getParameters().size() != 1 || name.length() > 9)
		return (ERR_ERRONEUSNICKNAME);
	for (std::string::iterator it = name.begin(), end = name.end(); it != end; ++it)
	{
		if (it == name.begin() && !this->isLetter(*it) && !this->isSpecial(*it))
			return (ERR_ERRONEUSNICKNAME);
		if (!this->isDigit(*it) && !this->isLetter(*it) && !this->isSpecial(*it))
			return (ERR_ERRONEUSNICKNAME);
	}
	return (0);
}

/*
 Command: USER
   Parameters: <user> <mode> <unused> <realname>

   The USER command is used at the beginning of connection to specify
   the username, hostname and realname of a new user.

   The <mode> parameter should be a numeric, and can be used to
   automatically set user modes when registering with the server.  This
   parameter is a bitmask, with only 2 bits having any signification: if
   the bit 2 is set, the user mode 'w' will be set and if the bit 3 is
   set, the user mode 'i' will be set.  (See Section 3.1.5 "User
   Modes").

   The <realname> may contain space characters.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
*/
int Client::USER(Command arguments)
{
	if (arguments.getParameters().size() < 5)
		return (ERR_NEEDMOREPARAMS);
	if (_clientType != TYPE_PASS)
		return (ERR_ALREADYREGISTRED);
	_clientType = TYPE_USER;
	std::string username = arguments.getParameters()[0];
	unsigned long i = 4;
	std::string realname = "";
	while (i < arguments.getParameters().size())
	{
		realname.append(arguments.getParameters().at(i));
		i++;
	}
	realname.erase(realname.find(":"), 1);
	if ((arguments.getParameters()[1].length() == 1))
		this->setUserMode(static_cast<unsigned char>(arguments.getParameters()[1][0]));
	this->_serverRef->changeClientClass(this, (new User(*this, username, realname)));
	return (0);
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
// ONLY USERS CAN BECOME OPERATORS
int Client::OPER(Command arguments)
{
	(void)arguments;
	return (ERR_NOOPERHOST);
}

/* Command: MODE
   Parameters: <nickname>
               *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )

   The user MODE's are typically changes which affect either how the
   client is seen by others or what 'extra' messages the client is sent.

   A user MODE command MUST only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.  If
   no other parameter is given, then the server will return the current
   settings for the nick.

      The available modes are as follows:

           a - user is flagged as away;
           i - marks a users as invisible;
           w - user receives wallops;
           r - restricted user connection;
           o - operator flag;
           O - local operator flag;
           s - marks a user for receipt of server notices.

   Additional modes may be available later on.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_USERSDONTMATCH
           ERR_UMODEUNKNOWNFLAG            RPL_UMODEIS

*/
int Client::MODE(Command arguments)
{
	// TODO : add the option for channels : redefinition in services responses
	/* Check params if enough */
	if (arguments.getParameters().size() < 3)
		return (ERR_NEEDMOREPARAMS);
	/* check if the parameters are correct (existing nickname, correctly writen params (exsting..))) */
	if (arguments.getParameters()[1][0] == '#')
		return (this->modeChannel(arguments));
	if (_nickname != arguments.getParameters()[1])
		return (ERR_USERSDONTMATCH);
	/* check if you can add them (are they already set) right user */
	if (arguments.getParameters()[2][0] == '-')
	{
		if ((_mode.begin() + _mode.find(arguments.getParameters()[2][1], 0)) == _mode.end())
			return (0);
		if (_availableModes.begin() + _availableModes.find(arguments.getParameters()[2][1], 0) == _availableModes.end())
			return (ERR_UMODEUNKNOWNFLAG);
		_mode.erase(_mode.find(arguments.getParameters()[2][1]), 1);
	}
	else if (arguments.getParameters()[2][0] == '+')
	{
		if ((_mode.begin() + _mode.find(arguments.getParameters()[2][1], 0)) != _mode.end())
			return (0);	
		if (_availableModes.begin() + _availableModes.find(arguments.getParameters()[2][1], 0) == _availableModes.end())
			return (ERR_UMODEUNKNOWNFLAG);
		_mode.push_back((arguments.getParameters()[2][1]));
	}
	return (0);
}

/*
	The <mode> parameter should be a numeric, and can be used to
   automatically set user modes when registering with the server.  This
   parameter is a bitmask, with only 2 bits having any signification: if
   the bit 2 is set, the user mode 'w' will be set and if the bit 3 is
   set, the user mode 'i' will be set.  (See Section 3.1.5 "User
   Modes").

*/
int Client::setUserMode(unsigned char num)
{
	if ((num & AWAY) == AWAY && _mode.find(AWAY) != std::string::npos)
		_mode.push_back(AWAY);
	else if ((num & INVISIBLE) == INVISIBLE && _mode.find(INVISIBLE) != std::string::npos)
		_mode.push_back(INVISIBLE);
	else if ((num & REC_WALLOPS) == REC_WALLOPS && _mode.find(REC_WALLOPS) != std::string::npos)
		_mode.push_back(REC_WALLOPS);
	else if ((num & RESTRICTED) == RESTRICTED && _mode.find(RESTRICTED) != std::string::npos)
		_mode.push_back(RESTRICTED);
	else if ((num & G_OPERATOR) == G_OPERATOR && _mode.find(G_OPERATOR) != std::string::npos)
		_mode.push_back(G_OPERATOR);
	else if ((num & LOC_OPERATOR) == LOC_OPERATOR && _mode.find(LOC_OPERATOR) != std::string::npos)
		_mode.push_back(LOC_OPERATOR);
	else if ((num & REC_USER_NOTICE) == REC_USER_NOTICE && _mode.find(REC_USER_NOTICE) != std::string::npos)
		_mode.push_back(REC_USER_NOTICE);
	return (0);
}

/*
  Command: SERVICE
   Parameters: <nickname> <reserved> <distribution> <type>
			   <reserved> <info>

   The SERVICE command to register a new service.  Command parameters
   specify the service nickname, distribution, type and info of a new
   service.

   The <distribution> parameter is used to specify the visibility of a
   service.  The service may only be known to servers which have a name
   matching the distribution.  For a matching server to have knowledge
   of the service, the network path between that server and the server
   on which the service is connected MUST be composed of servers which
   names all match the mask.

   The <type> parameter is currently reserved for future usage.

   Numeric Replies:

		   ERR_ALREADYREGISTRED            ERR_NEEDMOREPARAMS
		   ERR_ERRONEUSNICKNAME
		   RPL_YOURESERVICE                RPL_YOURHOST
		   RPL_MYINFO
*/
int Client::SERVICE(Command arguments)
{
	if (arguments.getParameters().size() < 7)	
		return (ERR_NEEDMOREPARAMS);
	if (_clientType == TYPE_SERVICE || _clientType == TYPE_USER)
		return (ERR_ALREADYREGISTRED);
	int retValNickname = this->checkNickname(arguments, arguments.getCommand());
	if (retValNickname != 0)
		return (retValNickname);
	_clientType = TYPE_SERVICE;
	return (RPL_YOURESERVICE);
}

/*
Command: QUIT
   Parameters: [ <Quit Message> ]

   A client session is terminated with a quit message.  The server
   acknowledges this by sending an ERROR message to the client.

   Numeric Replies:

           None.
*/
int Client::QUIT(Command arguments)
{
	(void)arguments;
	/* has to kill the obj, to avoid other commands to get executed*/
	return (0);
}


/*
Command: SQUIT
   Parameters: <server> <comment>

   The SQUIT command is available only to operators.  It is used to
   disconnect server links.  Also servers can generate SQUIT messages on
   error conditions.  A SQUIT message may also target a remote server
   connection.  In this case, the SQUIT message will simply be sent to
   the remote server without affecting the servers in between the
   operator and the remote server.

   The <comment> SHOULD be supplied by all operators who execute a SQUIT
   for a remote server.  The server ordered to disconnect its peer
   generates a WALLOPS message with <comment> included, so that other
   users may be aware of the reason of this action.

   Numeric replies:

           ERR_NOPRIVILEGES                ERR_NOSUCHSERVER
           ERR_NEEDMOREPARAMS
*/
int Client::SQUIT(Command arguments)
{
	(void)arguments;
	return (0);
}
