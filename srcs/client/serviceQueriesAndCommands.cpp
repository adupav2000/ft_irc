/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serviceQueriesAndCommands.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 22:27:47 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/26 20:14:31 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/*
 Command: SERVLIST
   Parameters: [ <mask> [ <type> ] ]

   The SERVLIST command is used to list services currently connected to
   the network and visible to the user issuing the command.  The
   optional parameters may be used to restrict the result of the query
   (to matching services names, and services type).

   Numeric Replies:

           RPL_SERVLIST                  RPL_SERVLISTEND
*/
int Client::SERVLIST(Command argument)
{
	(void)argument;
	return (0);
}

/*

      Command: SQUERY
   Parameters: <servicename> <text>

   The SQUERY command is used similarly to PRIVMSG.  The only difference
   is that the recipient MUST be a service.  This is the only way for a
   text message to be delivered to a service.

   See PRIVMSG for more details on replies and example.

   Examples:

   SQUERY irchelp :HELP privmsg
                                   ; Message to the service with
                                   nickname irchelp.

   SQUERY dict@irc.fr :fr2en blaireau
                                   ; Message to the service with name
                                   dict@irc.fr.
*/
int Client::SQUERY(Command argument)
{
	(void)argument;
	return (0);
}