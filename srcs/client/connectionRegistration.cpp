/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connectionRegistration.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 11:58:54 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/22 18:45:07 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "../server/server.hpp"
#include "../globals.hpp"

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
        (void)arguments;
	if (!arguments.getParameters().size() < 2)
		return (ERR_NONICKNAMEGIVEN);
	 if (_serverRef.nickNameUsed(arguments.getParameters()[1]))
	 	return (ERR_NICKNAMEINUSE);	
	 if (_mode.find('r'))
		return (ERR_RESTRICTED);
	std::string nTmp = arguments.getParameters()[1];
	/* Si jamais il y a plus de 2 arguments, cela veux dire qu'il 
	y a un espace */
	if (arguments.getParameters().size() != 2 || nTmp.length() > 9)
		return (ERR_ERRONEUSNICKNAME);
	for (std::string::iterator it = nTmp.begin(), end = nTmp.end(); it != end; ++it)
	{
		if (it == nTmp.begin() && !this->isLetter(*it) && !this->isSpecial(*it))
			return (ERR_ERRONEUSNICKNAME);
		if (!this->isDigit(*it) && !this->isLetter(*it) && !this->isSpecial(*it))
			return (ERR_ERRONEUSNICKNAME);
	}
        this->_nickname = arguments.getParameters()[0];
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
	if (_registered)
		return (ERR_ALREADYREGISTRED);
	// change the type with the needed function
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
// int Client::OPER(Command arguments)
// {
// 	if (arguments.getParameters().size() < 3)
// 		return (ERR_NEEDMOREPARAMS);
// 	if (arguments.getCommand()[2] != )
// 		return ();
// 	return (0);
// }

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
	(void)arguments;
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
	(void)arguments;
	return (0);
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
