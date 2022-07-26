/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelOperations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 13:59:57 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/25 19:27:18 by adu-pavi         ###   ########.fr       */
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
	Client *client = arguments->getClient();
	Channel *channel;
	std::vector<std::string> names = split(arguments->getParameters()[0], ",");
	for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
	{
		if (server->getChannel().size() == 0 || server->getChannel().find((*it)) == server->getChannel().end())
		{
			channel = new Channel((*it), server, client);
			server->addChannel(channel);
		}
		else
			channel = server->getChannel()[(*it)];		
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


/*
     Command: MODE
   Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )

   The MODE command is provided so that users may query and change the
   characteristics of a channel.  For more details on available modes
   and their uses, see "Internet Relay Chat: Channel Management" [IRC-
   CHAN].  Note that there is a maximum limit of three (3) changes per
   command for modes that take a parameter.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_KEYSET
           ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE
           RPL_CHANNELMODEIS
           RPL_BANLIST                     RPL_ENDOFBANLIST
           RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
           RPL_INVITELIST                  RPL_ENDOFINVITELIST
           RPL_UNIQOPIS
*/
// a faire si c'est un channel
// int Client::modeChannel(Command arguments)
// {
// 	_channels.changeMode(arguments);
// }
/*
int Client::MODE(Command arguments)
{
	(void)arguments;
	return (0);
}
*/



/*
    Command: TOPIC
   Parameters: <channel> [ <topic> ]

   The TOPIC command is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if this action is allowed for the user
   requesting it.  If the <topic> parameter is an empty string, the
   topic for that channel will be removed.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES
*/
int Client::TOPIC(Command arguments)
{
	(void)arguments;
	return (0);
}

/*
   Command: NAMES
   Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

   By using the NAMES command, a user can list all nicknames that are
   visible to him. For more details on what is visible and what is not,
   see "Internet Relay Chat: Channel Management" [IRC-CHAN].  The
   <channel> parameter specifies which channel(s) to return information
   about.  There is no error reply for bad channel names.

   If no <channel> parameter is given, a list of all channels and their
   occupants is returned.  At the end of this list, a list of users who
   are visible but either not on any channel or not on a visible channel
   are listed as being on `channel' "*".

   If the <target> parameter is specified, the request is forwarded to
   that server which will generate the reply.

   Wildcards are allowed in the <target> parameter.

   Numerics:

           ERR_TOOMANYMATCHES              ERR_NOSUCHSERVER
           RPL_NAMREPLY                    RPL_ENDOFNAMES
*/
int Client::NAME(Command arguments)
{
	(void)arguments;
	return (0);
}

/*
   Command: LIST
   Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

   The list command is used to list channels and their topics.  If the
   <channel> parameter is used, only the status of that channel is
   displayed.

   If the <target> parameter is specified, the request is forwarded to
   that server which will generate the reply.

   Wildcards are allowed in the <target> parameter.

   Numeric Replies:

           ERR_TOOMANYMATCHES              ERR_NOSUCHSERVER
           RPL_LIST                        RPL_LISTEND
*/
int Client::LIST(Command arguments)
{
	(void)arguments;
	return (0);
}

/*
   Command: INVITE
   Parameters: <nickname> <channel>

   The INVITE command is used to invite a user to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  However, if the channel exists, only members of the channel
   are allowed to invite other users.  When the channel has invite-only
   flag set, only channel operators may issue INVITE command.
   Only the user inviting and the user being invited will receive
   notification of the invitation.  Other channel members are not
   notified.  (This is unlike the MODE changes, and is occasionally the
   source of trouble for users.)

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_USERONCHANNEL
           ERR_CHANOPRIVSNEEDED
           RPL_INVITING                    RPL_AWAY
*/ 
int Client::INVITE(Command arguments)
{
	(void)arguments;
	return (0);
}

/*
   Command: KICK
   Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
               [<comment>]

   The KICK command can be used to request the forced removal of a user
   from a channel.  It causes the <user> to PART from the <channel> by
   force.  For the message to be syntactically correct, there MUST be
   either one channel parameter and multiple user parameter, or as many
   channel parameters as there are user parameters.  If a "comment" is
   given, this will be sent instead of the default message, the nickname
   of the user issuing the KICK.

   The server MUST NOT send KICK messages with multiple channels or
   users to clients.  This is necessarily to maintain backward
   compatibility with old client software.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL            ERR_NOTONCHANNEL
*/
int Client::KICK(Command arguments)
{
	(void)arguments;
	return (0);
}
