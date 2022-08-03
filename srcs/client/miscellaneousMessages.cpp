/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miscellaneousMessages.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamanfo <kamanfo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 22:23:58 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/08/03 18:43:22 by kamanfo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/* Command: KILL
   Parameters: <nickname> <comment>

   The KILL command is used to cause a client-server connection to be
   closed by the server which has the actual connection.  Servers
   generate KILL messages on nickname collisions.  It MAY also be
   available available to users who have the operator status.

   Clients which have automatic reconnect algorithms effectively make
   this command useless since the disconnection is only brief.  It does
   however break the flow of data and can be used to stop large amounts
   of 'flooding' from abusive users or accidents.  Abusive users usually
   don't care as they will reconnect promptly and resume their abusive
   behaviour.  To prevent this command from being abused, any user may
   elect to receive KILL messages generated for others to keep an 'eye'
   on would be trouble spots.

   In an arena where nicknames are REQUIRED to be globally unique at all
   times, KILL messages are sent whenever 'duplicates' are detected
   (that is an attempt to register two users with the same nickname) in
   the hope that both of them will disappear and only 1 reappear.

   When a client is removed as the result of a KILL message, the server
   SHOULD add the nickname to the list of unavailable nicknames in an
   attempt to avoid clients to reuse this name immediately which is
   usually the pattern of abusive behaviour often leading to useless
   "KILL loops".  See the "IRC Server Protocol" document [IRC-SERVER]
   for more information on this procedure.

   The comment given MUST reflect the actual reason for the KILL.  For
   server-generated KILLs it usually is made up of details concerning
   the origins of the two conflicting nicknames.  For users it is left
   up to them to provide an adequate reason to satisfy others who see
   it.  To prevent/discourage fake KILLs from being generated to hide
   the identify of the KILLer, the comment also shows a 'kill-path'
   which is updated by each server it passes through, each prepending
   its name to the path.

   Numeric Replies:

           ERR_NOPRIVILEGES              ERR_NEEDMOREPARAMS
           ERR_NOSUCHNICK                ERR_CANTKILLSERVER

   NOTE:
   It is RECOMMENDED that only Operators be allowed to kill other users
   with KILL command.  This command has been the subject of many
   controversies over the years, and along with the above
   recommendation, it is also widely recognized that not even operators
   should be allowed to kill users on remote servers.

*/
int Client::KILL(Command argument)
{
	(void)argument;
	return (0);
}

/*
      Command: PING
   Parameters: <server1> [ <server2> ]

   The PING command is used to test the presence of an active client or
   server at the other end of the connection.  Servers send a PING
   message at regular intervals if no other activity detected coming
   from a connection.  If a connection fails to respond to a PING
   message within a set amount of time, that connection is closed.  A
   PING message MAY be sent even if the connection is active.

   When a PING message is received, the appropriate PONG message MUST be
   sent as reply to <server1> (server which sent the PING message out)
   as soon as possible.  If the <server2> parameter is specified, it
   represents the target of the ping, and the message gets forwarded
   there.

   Numeric Replies:

           ERR_NOORIGIN                  ERR_NOSUCHSERVER

   Examples:

   PING tolsun.oulu.fi             ; Command to send a PING message to
                                   server

   PING WiZ tolsun.oulu.fi         ; Command from WiZ to send a PING
                                   message to server "tolsun.oulu.fi"

   PING :irc.funet.fi              ; Ping message sent by server
                                   "irc.funet.fi"
*/
int Client::PING(Command argument)
{
	if (argument.getParameters().size() == 0)
	    return ERR_NOORIGIN;
	std::string reply = ":" + getNickname() + " PONG \r\n";
	send(getPoll().fd, reply.c_str(), reply.size(), 0);
	return 0;
}

/*
      Command: PONG
   Parameters: <server> [ <server2> ]

   PONG message is a reply to ping message.  If parameter <server2> is
   given, this message MUST be forwarded to given target.  The <server>
   parameter is the name of the entity who has responded to PING message
   and generated this message.

   Numeric Replies:

           ERR_NOORIGIN                  ERR_NOSUCHSERVER

   Example:

   PONG csd.bu.edu tolsun.oulu.fi  ; PONG message from csd.bu.edu to
                                   tolsun.oulu.fi
*/
int Client::PONG(Command argument)
{
	(void)argument;
	return (0);
}
