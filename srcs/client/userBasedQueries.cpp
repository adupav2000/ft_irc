/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userBasedQueries.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 22:28:50 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/24 22:29:51 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/*
Command: WHO
   Parameters: [ <mask> [ "o" ] ]

   The WHO command is used by a client to generate a query which returns
   a list of information which 'matches' the <mask> parameter given by
   the client.  In the absence of the <mask> parameter, all visible
   (users who aren't invisible (user mode +i) and who don't have a
   common channel with the requesting client) are listed.  The same
   result can be achieved by using a <mask> of "0" or any wildcard which
   will end up matching every visible user.

   The <mask> passed to WHO is matched against users' host, server, real
   name and nickname if the channel <mask> cannot be found.

   If the "o" parameter is passed only operators are returned according
   to the <mask> supplied.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_WHOREPLY                  RPL_ENDOFWHO

   Examples:

   WHO *.fi                        ; Command to list all users who match
                                   against "*.fi".

   WHO jto* o                      ; Command to list all users with a
                                   match against "jto*" if they are an
                                   operator.

*/

int Client::WHO(Command arguments)
{
	(void)arguments;
	return 0;
}

/*
   Command: WHOIS
   Parameters: [ <target> ] <mask> *( "," <mask> )

   This command is used to query information about particular user.
   The server will answer this command with several numeric messages
   indicating different statuses of each user which matches the mask (if
   you are entitled to see them).  If no wildcard is present in the
   <mask>, any information about that nick which you are allowed to see
   is presented.

   If the <target> parameter is specified, it sends the query to a
   specific server.  It is useful if you want to know how long the user
   in question has been idle as only local server (i.e., the server the
   user is directly connected to) knows that information, while
   everything else is globally known.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

           ERR_NOSUCHSERVER              ERR_NONICKNAMEGIVEN
           RPL_WHOISUSER                 RPL_WHOISCHANNELS
           RPL_WHOISCHANNELS             RPL_WHOISSERVER
           RPL_AWAY                      RPL_WHOISOPERATOR
           RPL_WHOISIDLE                 ERR_NOSUCHNICK
           RPL_ENDOFWHOIS

   Examples:

   WHOIS wiz                       ; return available user information
                                   about nick WiZ

   WHOIS eff.org trillian          ; ask server eff.org for user
                                   information  about trillian
*/

int Client::WHOIS(Command arguments)
{
	std::string reply;
	if (arguments.getParameters().size() == 0)
		return ERR_NONICKNAMEGIVEN;
	if (_serverRef->nickNameUsed(arguments.getParameters()[0]) == false)
		return ERR_NOSUCHNICK;
	std::string chanNames;
	Client *user = _serverRef->findClientByNicknamme(arguments.getParameters()[0]);
	std::vector<Channel *> channels = user->getChannels();
	reply = ":" + getNickname() + "!" + getUsername() + "@localhost " + user->getNickname() + " " + user->getUsername() + " localhost * :" + user->getRealname() + "\r\n";
	send(getPoll().fd, reply.c_str(), reply.size(), 0);
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (user->getMode().find('i') == std::string::npos)
			chanNames = (*it)->getName() + " ";
	}
		
	if (chanNames.length())
	{
		reply = ":" + getNickname() + "!" + getUsername() + "@localhost " + user->getNickname() + " :" + chanNames + "\r\n";
		send(getPoll().fd, reply.c_str(), reply.size(), 0);
	}
	if (user->getMode().find('o') != std::string::npos)
	{
		reply = ":" + getNickname() + "!" + getUsername() + "@localhost " + user->getNickname() + " :is an IRC operator\r\n";
		send(getPoll().fd, reply.c_str(), reply.size(), 0);
	}
        sendReply(RPL_ENDOFWHOIS);
	// reply = ":" + getNickname() + "!" + getUsername() + "@localhost " + user->getNickname() + " :End of WHOIS list\r\n";
	// send(getPoll().fd, reply.c_str(), reply.size(), 0);
	return 0;
}

/*
   Command: WHOWAS
   Parameters: <nickname> *( "," <nickname> ) [ <count> [ <target> ] ]

   Whowas asks for information about a nickname which no longer exists.
   This may either be due to a nickname change or the user leaving IRC.
   In response to this query, the server searches through its nickname
   history, looking for any nicks which are lexically the same (no wild
   card matching here).  The history is searched backward, returning the
   most recent entry first.  If there are multiple entries, up to
   <count> replies will be returned (or all of them if no <count>
   parameter is given).  If a non-positive number is passed as being
   <count>, then a full search is done.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

           ERR_NONICKNAMEGIVEN           ERR_WASNOSUCHNICK
           RPL_WHOWASUSER                RPL_WHOISSERVER
           RPL_ENDOFWHOWAS

   Examples:

   WHOWAS Wiz                      ; return all information in the nick
                                   history about nick "WiZ";

   WHOWAS Mermaid 9                ; return at most, the 9 most recent
                                   entries in the nick history for
                                   "Mermaid";

   WHOWAS Trillian 1 *.edu         ; return the most recent history for
                                   "Trillian" from the first server
                                   found to match "*.edu"
*/