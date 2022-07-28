/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverQueriesAndCommands.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 22:23:58 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/28 07:44:58 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/*
Command: MOTD
   Parameters: [ <target> ]

   The MOTD command is used to get the "Message Of The Day" of the given
   server, or current server if <target> is omitted.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:
		   RPL_MOTDSTART                   RPL_MOTD
		   RPL_ENDOFMOTD                   ERR_NOMOTD

*/
int Client::MOTD(Command arguments)
{
	std::string errorStr;
	std::ifstream inputFile;
	std::string motdTxt = "";
	std::string line = "";

	// TODO Handle motd from other servers
	(void)arguments;
	this->sendReply(RPL_MOTDSTART);
	inputFile.open("./files/motd.txt");
	if (inputFile.is_open())
	{
		while (getline(inputFile, line))
		{
			motdTxt += line + '\n';
		}
		inputFile.close();
	}
	send(this->getPoll().fd, motdTxt.c_str(), motdTxt.size(), 0);
	this->sendReply(RPL_MOTD);
	return (0);
}

/*
	  Command: LUSERS
   Parameters: [ <mask> [ <target> ] ]

   The LUSERS command is used to get statistics about the size of the
   IRC network.  If no parameter is given, the reply will be about the
   whole net.  If a <mask> is specified, then the reply will only
   concern the part of the network formed by the servers matching the
   mask.  Finally, if the <target> parameter is specified, the request
   is forwarded to that server which will generate the reply.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

		   RPL_LUSERCLIENT                 RPL_LUSEROP
		   RPL_LUSERUNKOWN                 RPL_LUSERCHANNELS
		   RPL_LUSERME                     ERR_NOSUCHSERVER

*/
int Client::LUSERS(Command arguments)
{
	(void)arguments;
	return (RPL_LUSERCLIENT);
	return (RPL_LUSERUNKNOWN);
	return (RPL_LUSERME);
	return (RPL_LUSEROP);
	return (RPL_LUSERCHANNELS);
	return (ERR_NOSUCHSERVER);
}

/*
   Command: VERSION
   Parameters: [ <target> ]

   The VERSION command is used to query the version of the server
   program.  An optional parameter <target> is used to query the version
   of the server program which a client is not directly connected to.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

		   ERR_NOSUCHSERVER                RPL_VERSION

   Examples:

   VERSION tolsun.oulu.fi          ; Command to check the version of
								   server "tolsun.oulu.fi".
*/
int Client::VERSION(Command arguments)
{
	(void)arguments;
	return (ERR_NOSUCHSERVER);
	return (RPL_VERSION);
}

/*
   Command: STATS
   Parameters: [ <query> [ <target> ] ]

   The stats command is used to query statistics of certain server.  If
   <query> parameter is omitted, only the end of stats reply is sent
   back.

   A query may be given for any single letter which is only checked by
   the destination server and is otherwise passed on by intermediate
   servers, ignored and unaltered.

   Wildcards are allowed in the <target> parameter.

   Except for the ones below, the list of valid queries is
   implementation dependent.  The standard queries below SHOULD be
   supported by the server:

			l - returns a list of the server's connections, showing how
				long each connection has been established and the
				traffic over that connection in Kbytes and messages for
				each direction;
			m - returns the usage count for each of commands supported
				by the server; commands for which the usage count is
				zero MAY be omitted;
			o - returns a list of configured privileged users,
				operators;
			u - returns a string showing how long the server has been
				up.

   It is also RECOMMENDED that client and server access configuration be
   published this way.

   Numeric Replies:

		   ERR_NOSUCHSERVER
		   RPL_STATSLINKINFO                RPL_STATSUPTIME
		   RPL_STATSCOMMANDS                RPL_STATSOLINE
		   RPL_ENDOFSTATS

   Examples:

   STATS m                         ; Command to check the command usage
								   for the server you are connected to
*/
int Client::STATS(Command argument)
{
	(void)argument;
	return (0);
}

/*
   Command: LINKS
   Parameters: [ [ <remote server> ] <server mask> ]

   With LINKS, a user can list all servernames, which are known by the
   server answering the query.  The returned list of servers MUST match
   the mask, or if no mask is given, the full list is returned.

   If <remote server> is given in addition to <server mask>, the LINKS
   command is forwarded to the first server found that matches that name
   (if any), and that server is then required to answer the query.

   Numeric Replies:

		   ERR_NOSUCHSERVER
		   RPL_LINKS                        RPL_ENDOFLINKS

   Examples:

   LINKS *.au                      ; Command to list all servers which
								   have a name that matches *.au;

   LINKS *.edu *.bu.edu            ; Command to list servers matching
								   *.bu.edu as seen by the first server
								   matching *.edu.
*/
int Client::LINKS(Command argument)
{
	(void)argument;
	return (0);
}

/*
   Command: TIME
   Parameters: [ <target> ]

   The time command is used to query local time from the specified
   server. If the <target> parameter is not given, the server receiving
   the command must reply to the query.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

		   ERR_NOSUCHSERVER              RPL_TIME

   Examples:
   TIME tolsun.oulu.fi             ; check the time on the server
								   "tolson.oulu.fi"

*/
int Client::TIME(Command argument)
{
	(void)argument;
	return (0);
}

/*
   Command: CONNECT
   Parameters: <target server> <port> [ <remote server> ]

   The CONNECT command can be used to request a server to try to
   establish a new connection to another server immediately.  CONNECT is
   a privileged command and SHOULD be available only to IRC Operators.
   If a <remote server> is given and its mask doesn't match name of the
   parsing server, the CONNECT attempt is sent to the first match of
   remote server. Otherwise the CONNECT attempt is made by the server
   processing the request.

   The server receiving a remote CONNECT command SHOULD generate a
   WALLOPS message describing the source and target of the request.

   Numeric Replies:

		   ERR_NOSUCHSERVER              ERR_NOPRIVILEGES
		   ERR_NEEDMOREPARAMS
   Examples:

   CONNECT tolsun.oulu.fi 6667     ; Command to attempt to connect local
								   server to tolsun.oulu.fi on port 6667
*/
int Client::CONNECT(Command argument)
{
	(void)argument;
	return (0);
}

/*
   Command: TRACE
   Parameters: [ <target> ]

   TRACE command is used to find the route to specific server and
   information about its peers.  Each server that processes this command
   MUST report to the sender about it.  The replies from pass-through
   links form a chain, which shows route to destination.  After sending
   this reply back, the query MUST be sent to the next server until
   given <target> server is reached.

   TRACE command is used to find the route to specific server.  Each
   server that processes this message MUST tell the sender about it by
   sending a reply indicating it is a pass-through link, forming a chain
   of replies.  After sending this reply back, it MUST then send the
   TRACE message to the next server until given server is reached.  If
   the <target> parameter is omitted, it is RECOMMENDED that TRACE
   command sends a message to the sender telling which servers the local
   server has direct connection to.

   If the destination given by <target> is an actual server, the
   destination server is REQUIRED to report all servers, services and
   operators which are connected to it; if the command was issued by an
   operator, the server MAY also report all users which are connected to
   it.  If the destination given by <target> is a nickname, then only a
   reply for that nickname is given.  If the <target> parameter is
   omitted, it is RECOMMENDED that the TRACE command is parsed as
   targeted to the processing server.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

		   ERR_NOSUCHSERVER

	  If the TRACE message is destined for another server, all
	  intermediate servers must return a RPL_TRACELINK reply to indicate
	  that the TRACE passed through it and where it is going next.

		   RPL_TRACELINK

	  A TRACE reply may be composed of any number of the following
	  numeric replies.

		   RPL_TRACECONNECTING           RPL_TRACEHANDSHAKE
		   RPL_TRACEUNKNOWN              RPL_TRACEOPERATOR
		   RPL_TRACEUSER                 RPL_TRACESERVER
		   RPL_TRACESERVICE              RPL_TRACENEWTYPE
		   RPL_TRACECLASS                RPL_TRACELOG
		   RPL_TRACEEND
   Examples:

   TRACE *.oulu.fi                 ; TRACE to a server matching
								   *.oulu.fi
*/
int Client::TRACE(Command argument)
{
	(void)argument;
	return (0);
}

/*
   Command: ADMIN
   Parameters: [ <target> ]

   The admin command is used to find information about the administrator
   of the given server, or current server if <target> parameter is
   omitted.  Each server MUST have the ability to forward ADMIN messages
   to other servers.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

		   ERR_NOSUCHSERVER
		   RPL_ADMINME                   RPL_ADMINLOC1
		   RPL_ADMINLOC2                 RPL_ADMINEMAIL

   Examples:

   ADMIN tolsun.oulu.fi            ; request an ADMIN reply from
								   tolsun.oulu.fi

   ADMIN syrk                      ; ADMIN request for the server to
								   which the user syrk is connected
*/
int Client::ADMIN(Command argument)
{
	(void)argument;
	return (0);
}

/*
   Command: INFO
   Parameters: [ <target> ]

   The INFO command is REQUIRED to return information describing the
   server: its version, when it was compiled, the patchlevel, when it
   was started, and any other miscellaneous information which may be
   considered to be relevant.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

		   ERR_NOSUCHSERVER
		   RPL_INFO                      RPL_ENDOFINFO

   Examples:

   INFO csd.bu.edu                 ; request an INFO reply from
								   csd.bu.edu

   INFO Angel                      ; request info from the server that
								   Angel is connected to.
*/
int Client::INFO(Command argument)
{
	(void)argument;
	return (0);
}