/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendingMessages.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 22:21:48 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/08/05 00:50:36 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/*
   Command: PRIVMSG
   Parameters: <msgtarget> <text to be sent>

   PRIVMSG is used to send private messages between users, as well as to
   send messages to channels.  <msgtarget> is usually the nickname of
   the recipient of the message, or a channel name.

   The <msgtarget> parameter may also be a host mask (#<mask>) or server
   mask ($<mask>).  In both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask MUST
   have at least 1 (one) "." in it and no wildcards following the last
   ".".  This requirement exists to prevent people sending messages to
   "#*" or "$*", which would broadcast to all users.  Wildcards are the
   '*' and '?'  characters.  This extension to the PRIVMSG command is
   only available to operators.

   Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY

   Examples:

   :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
                                   ; Message from Angel to Wiz.

   PRIVMSG Angel :yes I'm receiving it !
                                   ; Command to send a message to Angel.

   PRIVMSG jto@tolsun.oulu.fi :Hello !
                                   ; Command to send a message to a user
                                   on server tolsun.oulu.fi with
                                   username of "jto".

   PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
                                   ; Message to a user on server
                                   irc.stealth.net wgloith username of
                                   "kalt", and connected from the host
                                   millennium.stealth.net.

   PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
                                   ; Message to a user on the local
                                   server with username of "kalt", and
                                   connected from the host
                                   millennium.stealth.net.

   PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
                                   ; Message to the user with nickname
                                   Wiz who is connected from the host
                                   tolsun.oulu.fi and has the username
                                   "jto".

   PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
                                   ; Message to everyone on a server
                                   which has a name matching *.fi.

   PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
                                   ; Message to all users who come from
                                   a host which has a name matching
                                   *.edu.

*/

void privmsgUser(Command *arguments, std::string receiver, Server *server, std::string message)
{
	std::string reply;
	std::map<int, Client *> clients = server->getClients();
	for(std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if ((*it).second->getNickname() == receiver)
		{
			reply = ":" + arguments->getClient()->getNickname() + "!" + arguments->getClient()->getUsername() + "@localhost PRIVMSG " + receiver + " " + message + "\r\n";
            send((*it).first, reply.c_str() , reply.size(), 0);
			return ;
        }
    }
	reply = receiver + " :No such nick/channel\r\n";
	send(arguments->getClient()->getPoll().fd, reply.c_str() , reply.size(), 0);
    return ;
}

void privmsgChannel(Command *arguments, std::string receiver, Server *server, std::string message)
{
    std::string reply;
	std::map<std::string, Channel *> channels = server->getChannel();
	std::map<std::string, Channel *>::iterator it;
	if ((it = channels.find(receiver)) != channels.end())
	{
        if (it->second->clientOnChannel(arguments->getClient()->getNickname()) == false)
            return ;
		reply = ":" + arguments->getClient()->getNickname() + "!" + arguments->getClient()->getUsername() + "@localhost PRIVMSG " + receiver + " " + message + "\r\n";
    	std::map<int, Client *> channelClients = (*it).second->getClients();
		for (std::map<int, Client *>::iterator cli = channelClients.begin(); cli != channelClients.end(); cli++)
		{
			if (cli->first != arguments->getClient()->getPoll().fd)
				send((*cli).first, reply.c_str() , reply.size(), 0);
		}
		return ;
	}
	else
	{
		reply = receiver + " :No such nick/channel\r\n";
		send(arguments->getClient()->getPoll().fd, reply.c_str() , reply.size(), 0);
	}
    return;
}

int Client::PRIVMSG(Command arguments)
{
	if (arguments.getParameters().size() == 0)
		return ERR_NEEDMOREPARAMS;
	else if (arguments.getParameters().size() == 1)
		return ERR_NOTEXTTOSEND;
	std::string receiver = arguments.getParameters()[0];
	Server *server = arguments.getServer();
    if (receiver.find("#") != std::string::npos)
        privmsgChannel(&arguments, receiver, server, arguments.getMessage());
    else 
    {
        privmsgUser(&arguments, receiver, server, arguments.getMessage());
    }
	return 0;
}

/*
      Command: NOTICE
   Parameters: <msgtarget> <text>

   The NOTICE command is used similarly to PRIVMSG.  The difference
   between NOTICE and PRIVMSG is that automatic replies MUST NEVER be
   sent in response to a NOTICE message.  This rule applies to servers
   too - they MUST NOT send any error reply back to the client on
   receipt of a notice.  The object of this rule is to avoid loops
   between clients automatically sending something in response to
   something it received.

   This command is available to services as well as users.

   This is typically used by services, and automatons (clients with
   either an AI or other interactive program controlling their actions).

   See PRIVMSG for more details on replies and examples.
*/
void noticeUser(Command *arguments, std::string receiver, Server *server, std::string message)
{
	std::string reply;
	std::map<int, Client *> clients = server->getClients();
	for(std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if ((*it).second->getNickname() == receiver)
		{
			reply = ":" + arguments->getClient()->getNickname() + "!" + arguments->getClient()->getUsername() + "@localhost NOTICE " + receiver + " " + message + "\r\n";
            send((*it).first, reply.c_str() , reply.size(), 0);
			return ;
        }
    }
	reply = receiver + " :No such nick/channel\r\n";
	send(arguments->getClient()->getPoll().fd, reply.c_str() , reply.size(), 0);
    return ;
}

void noticeChannel(Command *arguments, std::string receiver, Server *server, std::string message)
{
    std::string reply;
	std::map<std::string, Channel *> channels = server->getChannel();
	std::map<std::string, Channel *>::iterator it;
	if ((it = channels.find(receiver)) != channels.end())
	{
        if (it->second->clientOnChannel(arguments->getClient()->getNickname()) == false)
            return ;
		reply = ":" + arguments->getClient()->getNickname() + "!" + arguments->getClient()->getUsername() + "@localhost NOTICE " + receiver + " " + message + "\r\n";
    	std::map<int, Client *> channelClients = (*it).second->getClients();
		for (std::map<int, Client *>::iterator cli = channelClients.begin(); cli != channelClients.end(); cli++)
		{
			if (cli->first != arguments->getClient()->getPoll().fd)
				send((*cli).first, reply.c_str() , reply.size(), 0);
		}
		return ;
	}
	else
	{
		reply = receiver + " :No such nick/channel\r\n";
		send(arguments->getClient()->getPoll().fd, reply.c_str() , reply.size(), 0);
	}
    return;
}
int Client::NOTICE(Command arguments)
{
	(void)arguments;
	if (arguments.getParameters().size() == 0)
		return ERR_NEEDMOREPARAMS;
	else if (arguments.getParameters().size() == 1)
		return ERR_NOTEXTTOSEND;
	std::string receiver = arguments.getParameters()[0];
	Server *server = arguments.getServer();
    if (receiver.find("#") != std::string::npos)
        noticeChannel(&arguments, receiver, server, arguments.getMessage());
    else 
    {
        noticeUser(&arguments, receiver, server, arguments.getMessage());
    }
}