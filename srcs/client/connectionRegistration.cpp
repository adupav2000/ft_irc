/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionRegistration.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamanfo <kamanfo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 11:58:54 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/08/03 19:05:16 by kamanfo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "../server/server.hpp"
#include "../globals.hpp"

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
	if (_clientStatus == CONNECTED)
		return (ERR_ALREADYREGISTRED);
	if (!(_serverRef->correctPassword(arguments.getParameters()[0])))
		setStatus(REFUSED);
	else
		_passOK = true;
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
	if (_mode.find('r') != std::string::npos)
		return (ERR_RESTRICTED);
	std::string nTmp = arguments.getParameters()[0];
	std::string reply;
	int retValNickname;
	retValNickname = this->checkNickname(arguments, nTmp);
	if (retValNickname != 0)
		return (retValNickname);
	if (_clientStatus == PENDING)
	{
	
		// CAS OU LE CLIENT N'EST PAS ENCORE ENREGISTRÉ
		while (_serverRef->nickNameUsed(nTmp))
			nTmp += "_";
	}
	else
	{
		if (_serverRef->nickNameUsed(arguments.getParameters()[0]))
			return (ERR_NICKNAMEINUSE);	
	}
	_clientType = TYPE_CLIENT;
	reply = ":" + getNickname() + "!" + getUsername() + "@localhost NICK " + nTmp + "\r\n";
	this->_nickname = nTmp;
	send(getPoll().fd, reply.c_str(), reply.size(), 0);
	return 0;
}

int	Client::checkNickname(Command arguments, std::string name) const
{
	if (arguments.getParameters().size() < 1 || name.length() > 9)
	{
		std::cout << "issue at stage 0 size " << arguments.getParameters().size() << "len " << name.length() << std::endl;
		return (ERR_ERRONEUSNICKNAME);
	}
	for (std::string::iterator it = name.begin(), end = name.end(); it != end; ++it)
	{
		if (it == name.begin() && !this->isLetter(*it) && !this->isSpecial(*it))
		{
			std::cout << "First issue here " << !this->isLetter(*it) << !this->isSpecial(*it) << std::endl;
			return (ERR_ERRONEUSNICKNAME);
		}
		if (!this->isDigit(*it) && !this->isLetter(*it) && !this->isSpecial(*it))
		{
			std::cout << "Second issue here with char " << *it << "->" << !this->isDigit(*it) << !this->isLetter(*it) << !this->isSpecial(*it) << std::endl;
			return (ERR_ERRONEUSNICKNAME);
		}
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
	if (arguments.getParameters().size() < 3)
		return (ERR_NEEDMOREPARAMS);
	if (_clientStatus == CONNECTED)
		return (ERR_ALREADYREGISTRED);
	_clientType = TYPE_USER;
	_username = arguments.getParameters()[0];
	_hostname = arguments.getParameters()[2];
	_realname = arguments.getMessage();
	if ((arguments.getParameters()[1].length() == 1))
		this->setUserMode(static_cast<unsigned char>(arguments.getParameters()[1][0]));
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
	std::string reply;
	std::string name = "oper";
	std::string password = "pass";

	if (arguments.getParameters().size() < 2)
		return ERR_NEEDMOREPARAMS;
	if (arguments.getParameters()[0] != name)
		return ERR_NOOPERHOST;
	if (arguments.getParameters()[1] != password)
		return ERR_PASSWDMISMATCH;
	this->sendReply(RPL_YOUREOPER);
	// reply = ":" + getNickname() + "!" + getUsername() + "@localhost You are now an IRC operator\r\n";
	send(getPoll().fd, reply.c_str(), reply.size(), 0);
	if (getMode().find('o') == std::string::npos)
	{
		_mode = getMode() + "o";
		reply = ":" + getNickname() + "!" + getUsername() + "@localhost Your user mode is [+" + _mode + "]\r\n";
		send(getPoll().fd, reply.c_str(), reply.size(), 0);
	}
	return 0;
}

/* Command: MODE
   Parameters: <nickname>
               *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )

   The user MODE's are typically changes which affect either how the
   client is seen by others or what 'extra' messages the client is sent.

   A user MODE command MUST only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.  If
   no other parameter is given, then the server will return the current
   settings for the nick.r

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
	std::string reply;

	if (arguments.getParameters().size() < 1)
		return (ERR_NEEDMOREPARAMS);
	//todo sijamais le nom n'a pas de # mais est un nom de channel on fait quoi ?
	if (arguments.getParameters()[0][0] == '#')
		return (this->modeChannel(arguments));
	if (_nickname != arguments.getParameters()[0])
		return (ERR_USERSDONTMATCH);
	if (arguments.getParameters().size() > 1)
	{
		for (size_t i = 1; i < arguments.getParameters()[1].size(); i++)
		{
			if (_availableModes.find(arguments.getParameters()[1][i]) == std::string::npos)
				return (ERR_UMODEUNKNOWNFLAG);
		}
		if (arguments.getParameters()[1][0] == '-')
		{
			for (size_t i = 1; i < arguments.getParameters()[1].size(); i++)
			{
				if (_mode.find((arguments.getParameters()[1][i])) == std::string::npos || arguments.getParameters()[1][i] == 'a' || arguments.getParameters()[1][i] == 'r')
					continue;
				_mode.erase(_mode.find(arguments.getParameters()[1][i]), 1);
			}	
		}
		else if ((arguments.getParameters()[1])[0] == '+')
		{
			for (size_t i = 1; i < arguments.getParameters()[1].size(); i++)
			{
				if (_mode.find((arguments.getParameters()[1][i])) != std::string::npos || _availableModes.find((arguments.getParameters()[1][i])) == std::string::npos || arguments.getParameters()[1][i] == 'a' || arguments.getParameters()[1][i] == 'o' || arguments.getParameters()[1][i] == 'O')
					continue;
				_mode.push_back((arguments.getParameters()[1][i]));
			}
		}
	}
	reply = ":" + getNickname() + "!" + getUsername() + "@localhost Your user mode is +" + _mode + "\r\n";
	send(getPoll().fd, reply.c_str(), reply.size(), 0);
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
	Parameters: none

   A client session is terminated with a quit message.  The server
   acknowledges this by sending an ERROR message to the client.

   Numeric Replies:

           None.
*/
int Client::QUIT(Command arguments)
{
	(void)arguments;
	_clientStatus = DISCONNECTED;
	sendReply(SEND_CONFIRMDISCONNECT);
	this->getServer()->removeClient(this->getPoll().fd);
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
