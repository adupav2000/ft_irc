/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelOperations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 13:59:57 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/22 17:11:27 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

/*
Command: JOIN
   Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
			   / "0"

   The JOIN command is used by a user to request to start listening to
   the specific channel.  Servers MUST be able to parse arguments in the
   form of a list of target, but SHOULD NOT use lists when sending JOIN
   messages to clients.

   Once a user has joined a channel, he receives information about
   all commands his server receives affecting the channel.  This
   includes JOIN, MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.
   This allows channel members to keep track of the other channel
   members, as well as channel modes.

   If a JOIN is successful, the user receives a JOIN message as
   confirmation and is then sent the channel's topic (using RPL_TOPIC) and
   the list of users who are on the channel (using RPL_NAMREPLY), which
   MUST include the user joining.

   Note that this message accepts a special argument ("0"), which is
   a special request to leave all channels the user is currently a member
   of.  The server will process this message as if the user had sent
   a PART command (See Section 3.2.2) for each channel he is a member
   of.

   Numeric Replies:

		   ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
		   ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
		   ERR_CHANNELISFULL               ERR_BADCHANMASK
		   ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
		   ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
		   RPL_TOPIC
*/
int JOIN(Command *arguments)
{
	std::string reply;
	Server *server = arguments->getServer();
	std::cout << "server : " << server->getChannel().size() <<std::endl;
	Client *client = arguments->getClient();
	Channel *channel;
	std::vector<std::string> names = split(arguments->getParameters()[0], ",");
	for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
	{
		std::cout << "channel name : " << (*it) << std::endl;
		channel = new Channel((*it), server, client);
		server->addChannel(channel);
		channel->addToChannel(client);
		client->setChannel(channel);
	}
	if (arguments->getParameters().size() > 1)
	{
		std::vector<std::string> keys = split(arguments->getParameters()[1], ",");
		for (size_t i = 0; i < keys.size(); i++)
		{
			std::cout << "keys" << keys[i] << std::endl;
			(server->getChannel()[names[i]])->setKey(keys[i]);
		}
	}
	reply += ":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " JOIN " + names[0] + "\r\n";
	send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	channel = server->getChannel()[names[0]];
	if (channel->getTopic().size() > 0)
	{
		reply = names[0] + " :" + channel->getTopic();
		send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	}
	reply = channel->getMode() + " " + names[0] + " :" + channel->getClientsName(channel) + "\r\n";
	send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	reply = channel->getName() + ":End of /NAMES list\r\n";
	send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	return (0);
}


/*
  Command: PART
   Parameters: <channel> *( "," <channel> ) [ <Part Message> ]

   The PART command causes the user sending the message to be removed
   from the list of active members for all given channels listed in the
   parameter string.  If a "Part Message" is given, this will be sent
   instead of the default message, the nickname.  This request is always
   granted by the server.

   Servers MUST be able to parse arguments in the form of a list of
   target, but SHOULD NOT use lists when sending PART messages to
   clients.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_NOTONCHANNEL
*/
int Channel::PART(Command *arguments)
{
	(void)arguments;
	return (0);
}