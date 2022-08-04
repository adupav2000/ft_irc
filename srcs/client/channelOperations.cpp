/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelOperations.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 13:59:57 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/26 14:06:16by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "../channel/channel.hpp"



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
int Client::JOIN(Command arguments)
{
	std::string reply;
	Server *server = arguments.getServer();
	Client *client = arguments.getClient();
	Channel *channel;
	std::vector<Channel *> channels = getChannels();
	if (arguments.getParameters()[0] == "#0")
	{
		for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " PART " + (*it)->getName() + "\r\n";
			std::map<int, Client *> users = (*it)->getClients();
			for (std::map<int, Client *>::iterator cli = users.begin() ; cli != users.end(); cli++)
			{
				send(cli->first, reply.c_str(), reply.size(), 0);
			}
			(*it)->removeFromChannel(client);
			client->leaveChannel((*it));
			if ((*it)->getClients().size() == 0)
				server->destroyChannel((*it));
		}
		return 0;
	}
	std::vector<std::string> names = split(arguments.getParameters()[0], ",");
	for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
	{
		if (!server->getChannel().count(*it))
		{
			channel = new Channel((*it), server, client);
			server->addChannel(channel);
			channel->changeUserMode(getPoll().fd, 'o', '+');
		}
		else
		{
			channel = server->getChannel().find(*it)->second;
			if (channel->getMode().find('l') != std::string::npos && channel->getClients().size() == channel->getMaxClients())
			{
				reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost " + channel->getName() + " :Cannot join channel (+l) \r\n";
				send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
				return 0;
			}
			if (channel->getMode().find('i') != std::string::npos && channel->isInvited(getNickname()) == false)
			{
				reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost " + channel->getName() + " :Cannot join channel (+i)\r\n";
				send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
				return 0;
			}
		}
		channel->addToChannel(client);
		client->setChannel(channel);
	}
	if (arguments.getParameters().size() > 1)
	{
		std::vector<std::string> keys = split(arguments.getParameters()[1], ",");
		for (size_t i = 0; i < keys.size(); i++)
		{
			(server->getChannel()[names[i]])->setKey(keys[i], '+');
		}
	}
	reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " JOIN " + names[0] + "\r\n";
	send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	channel = server->getChannel()[names[0]];
	if (channel->getTopic().size() > 0)
	{
		reply = names[0] + " :" + channel->getTopic();
		send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	}
	reply = channel->getMode() + " " + names[0] + " :" + channel->getClientsName(channel) + "\r\n";
	send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	reply = channel->getName() + "End of /NAMES list\r\n";
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

int Client::PART(Command arguments)
{
	std::string reply;
	Server *server = arguments.getServer();
	// Client *client = arguments.getClient();
	Channel *channel;

	if (arguments.getParameters().size() == 0)
		return ERR_NEEDMOREPARAMS;
	std::vector<std::string> names = split(arguments.getParameters()[0], ",");
	for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it)
	{
		if (server->getChannel().count(*it))
		{
			channel = server->getChannel()[*it];
			if (!channel->getClients().count(this->getPoll().fd))
			{
				reply = channel->getName() + " :You're not on that channel";
				send(this->getPoll().fd, reply.c_str(), reply.size(), 0);
				continue;
			}
			std::cout << this->getNickname() << " is quitting" << std::endl;
			if (!(arguments.getParameters().size() == 2 && arguments.getParameters()[1] == this->_nickname))
				reply = ":" + this->getNickname() + "!" + this->getUsername() + "@localhost" + " PART " + channel->getName() + " :" + arguments.getMessage() + "\r\n";
			std::map<int, Client *> users = channel->getClients();
			for (std::map<int, Client *>::iterator cli = users.begin() ; cli != users.end(); cli++)
			{
				send(cli->first, reply.c_str(), reply.size(), 0);
			}
			channel->removeFromChannel(this);
			this->leaveChannel(channel);
			if (channel->getClients().size() == 0)
				server->destroyChannel(channel);
		}
		else
		{
			reply = channel->getName() + " :No such channel";
			send(this->getPoll().fd, reply.c_str(), reply.size(), 0);
		}
	}
	return 0;
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
int Client::modeChannel(Command arguments)
{
	std::string reply;
	std::string setMode = "klbeI";
	std::string giveMode = "Oov";
	std::string toggleMode = "aimnqpsrt";
	std::string param = "";
	//_channel->changeMode(arguments);
	if (arguments.getParameters().size() < 1)
		return ERR_NEEDMOREPARAMS;
	if (_serverRef->getChannel().count(arguments.getParameters()[0]) == 0)
		return ERR_NOSUCHCHANNEL;
	Channel *channel = _serverRef->getChannel()[arguments.getParameters()[0]];
	if (arguments.getParameters().size() > 1)
	{
		for (size_t i = 1; i < arguments.getParameters()[1].size(); i++)
		{
			if (channel->getChannelModes().find(arguments.getParameters()[1][i]) == std::string::npos)
				return (ERR_UMODEUNKNOWNFLAG);
		}
		for (size_t i = 1; i < arguments.getParameters()[1].size(); i++)
		{
			if (arguments.getParameters()[1][0] == '-')
			{
				if (channel->getMode().find((arguments.getParameters()[1][i])) == std::string::npos)
						continue;
			}
			else if ((arguments.getParameters()[1])[0] == '+')
			{
				if (channel->getMode().find((arguments.getParameters()[1][i])) != std::string::npos || channel->getChannelModes().find((arguments.getParameters()[1][i])) == std::string::npos)
					continue;
			}
			if (getMode().find("o") != std::string::npos || channel->getUserMode()[getPoll().fd].find("O") != std::string::npos || channel->getUserMode()[getPoll().fd].find("o") != std::string::npos)
			{
				if (toggleMode.find(arguments.getParameters()[1][i]) != std::string::npos)
					channel->setMode(arguments.getParameters()[1][i], arguments.getParameters()[1][0]);
				else
				{
					if (arguments.getParameters().size() < 3)
					{
						reply = ":" + getNickname() + " MODE :Not enough parameters\r\n";
						send(getPoll().fd, reply.c_str(), reply.size(), 0);
					}
					else 
					{
						if (giveMode.find(arguments.getParameters()[1][i]) != std::string::npos)
						{
							if (arguments.getParameters().size() < 3)
							{
								reply = ":" + getNickname() + " MODE :Not enough parameters\r\n";
								send(getPoll().fd, reply.c_str(), reply.size(), 0);
							}
							else
							{
								// MODE #chan +o adu-pavi

								if (channel->clientOnChannel(arguments.getParameters()[2]))
								{
									Client *target = channel->getClientOnChannel(arguments.getParameters()[2]);
									channel->changeUserMode(target->getPoll().fd, arguments.getParameters()[1][i], arguments.getParameters()[1][0]);
								}
								else
								{
									reply = ":" + getNickname() + " " + channel->getClientOnChannel(arguments.getParameters()[2])->getNickname() + " " + channel->getName() + " :They aren't on that channel\r\n";
									send(getPoll().fd, reply.c_str(), reply.size(), 0);
								}
							}
						}
						else if (setMode.find(arguments.getParameters()[1][i]) != std::string::npos)
						{
							if (arguments.getParameters()[1][i] == 'k')
							{
								if (arguments.getParameters()[2] == channel->getKey())
									channel->setKey(arguments.getParameters()[2], arguments.getParameters()[1][0]);
								param = channel->getKey();
							}
							else if (arguments.getParameters()[1][i] == 'l')
							{
								if (atoi((arguments.getParameters()[2]).c_str()) > 1)
									channel->setMaxClients(atoi((arguments.getParameters()[2]).c_str()));
								channel->getMaxClients();
							}
						}
					}
				}
			}
			else
			{
				reply = getNickname() + " " + channel->getName() + " :You're not channel operator\r\n";
				send(getPoll().fd, reply.c_str(), reply.size(), 0);
			}

		}
	}
	reply = ":" + getNickname() + "!" + getUsername() + "@localhost " + channel->getName() + " " + channel->getMode() + " " + param + "\r\n";
	send(getPoll().fd, reply.c_str(), reply.size(), 0);
	return 0;
}

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
	std::string reply;
	Client *client = arguments.getClient();
	Channel *channel;

	if (arguments.getParameters().size() == 0)
		return ERR_NEEDMOREPARAMS;
	if (_serverRef->getChannel().count(arguments.getParameters()[0]) == 0)
		return ERR_NOSUCHCHANNEL;
	channel = _serverRef->getChannel()[arguments.getParameters()[0]];
	if (!channel->getClients().count(client->getPoll().fd))
		return ERR_NOTONCHANNEL;
	if (arguments.getMessage() == "")
	{
		if (channel->getTopic() == "")
		{
			reply = channel->getName() + " :No topic is set";
			send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
			return 0;
		}
		else
		{
			reply = channel->getName() + " :" + channel->getTopic();
			send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
			return 0;
		}
	}
	else 
	{
		std::cout << arguments.getMessage() << std::endl;
		if (channel->getMode().find('t') != std::string::npos && client->getMode().find('o') == std::string::npos && channel->getUserMode()[getPoll().fd].find("O") == std::string::npos && channel->getUserMode()[getPoll().fd].find("o") == std::string::npos)
			return ERR_CHANOPRIVSNEEDED;
		if (arguments.getMessage() == ":")
			channel->setTopic("");
		else
			channel->setTopic(arguments.getMessage());
	}
	std::cout << arguments.getMessage() << std::endl;
	reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " TOPIC " + channel->getName() + " :" + arguments.getMessage() + "\r\n";
	std::map<int, Client *> users = channel->getClients();
	for (std::map<int, Client *>::iterator cli = users.begin() ; cli != users.end(); cli++)
		send(cli->first, reply.c_str(), reply.size(), 0);
	return 0;
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
	std::string reply;
	Server *server = arguments.getServer();
	Client *client = arguments.getClient();
	std::map<int, Client *> clients;
	std::map<std::string, Channel *> channels = server->getChannel();

	if (arguments.getParameters().size() == 1)
	{
		std::vector<std::string> param = split(arguments.getParameters()[0], ",");
		for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			for (std::vector<std::string>::iterator paramIt = param.begin(); paramIt < param.end(); paramIt++)
			{
				if (it->second->getName() == *paramIt)
				{
					std::string mode;
					if (it->second->getMode().find('p') != std::string::npos)
						mode = "*";
					else if (it->second->getMode().find('s') != std::string::npos)
						mode = "@";
					else
						mode = "=";
					reply = ":" + client->getNickname() + " " + mode + " " + it->second->getName();
					clients = it->second->getClients();
					if (clients.size())
					{
						for (std::map<int, Client *>::iterator cli = clients.begin(); cli != clients.end(); cli++)
						{
							if (cli->second->getMode().find('i') != std::string::npos)
								continue;
							if (cli != clients.begin())
								reply += " ";
							if (cli->second->getMode().find("o") != std::string::npos || it->second->getUserMode()[cli->second->getPoll().fd].find("O") != std::string::npos || it->second->getUserMode()[cli->second->getPoll().fd].find("o") != std::string::npos)
								reply += "@";
							reply += cli->second->getNickname();
						}
						reply += "\r\n";
						send(arguments.getClient()->getPoll().fd, reply.c_str(), reply.size(), 0);
					}
				}
			}
			reply = it->second->getName() + " :End of /NAMES list\r\n";
			send(arguments.getClient()->getPoll().fd, reply.c_str(), reply.size(), 0);
		}
	}
	else
	{
		for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			std::string mode;
			if (it->second->getMode().find('p') != std::string::npos)
				mode = "*";
			else if (it->second->getMode().find('s') != std::string::npos)
				mode = "@";
			else
				mode = "=";
			reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost " + mode + " " + it->second->getName();
			clients = it->second->getClients();
			if (clients.size())
			{
				for (std::map<int, Client *>::iterator cli = clients.begin(); cli != clients.end(); cli++)
				{
					if (cli->second->getMode().find('i') != std::string::npos)
						continue;
					if (cli != clients.begin())
						reply += " ";
					if (cli->second->getMode().find("o") != std::string::npos || it->second->getUserMode()[cli->second->getPoll().fd].find("O") != std::string::npos || it->second->getUserMode()[cli->second->getPoll().fd].find("o") != std::string::npos)
						reply += "@";
					reply += cli->second->getNickname();
				}
				reply += "\r\n";
				send(arguments.getClient()->getPoll().fd, reply.c_str(), reply.size(), 0);
			}
			reply = it->second->getName() + " :End of /NAMES list\r\n";
			send(arguments.getClient()->getPoll().fd, reply.c_str(), reply.size(), 0);	
		}
		reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost *";
		std::map<int, Client *> users = server->getClients();
		if (users.size())
		{
			for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); ++it)
			{
				if (it->second->getChannels().size() == 0)
				{
					reply += " ";
					reply += it->second->getNickname();
				}
			}
			reply += "\r\n";
			send(arguments.getClient()->getPoll().fd, reply.c_str(), reply.size(), 0);
		}
		reply = "* :End of /NAMES list\r\n";
		send(arguments.getClient()->getPoll().fd, reply.c_str(), reply.size(), 0);	
	}
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
	std::string reply;
	Server *server = arguments.getServer();
	Client *client = arguments.getClient();
	std::map<std::string, Channel *> channels = server->getChannel();

	if (arguments.getParameters().size() > 0 && arguments.getParameters()[0] != "")
	{
		std::vector<std::string> param = split(arguments.getParameters()[0], ",");
		for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			for (std::vector<std::string>::iterator paramIt = param.begin(); paramIt < param.end(); paramIt++)
			{
				if (arguments.getParameters().size() == 0 || it->second->getName() == *paramIt)
				{
					reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost " + it->second->getName() + " " + patch::to_string(it->second->getClients().size()) + " " + it->second->getTopic() + "\r\n";
					send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
				}
			}
		}
	}
	else 
	{
		for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost " + it->second->getName() + " " + patch::to_string(it->second->getClients().size()) + " " + it->second->getTopic() + "\r\n";
			send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
		}
	}
	reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost End of /LIST\r\n";
	send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	return 0;
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
	std::string reply;
	Server *server = arguments.getServer();
	Client *client = arguments.getClient();
	Channel *channel;

	if (arguments.getParameters().size() < 2)
		return ERR_NEEDMOREPARAMS;
	if (server->nickNameUsed(arguments.getParameters()[0]) == false)
		return ERR_NOSUCHNICK;
	if (server->getChannel().count(arguments.getParameters()[1]))
	{
		channel = server->getChannel()[arguments.getParameters()[1]];
		if ((getMode().find("o") == std::string::npos && channel->getUserMode()[getPoll().fd].find("O") == std::string::npos && channel->getUserMode()[getPoll().fd].find("o") == std::string::npos) && channel->getMode().find('i') != std::string::npos)
			return ERR_CHANOPRIVSNEEDED;
		if (server->getChannel()[arguments.getParameters()[1]]->clientOnChannel(arguments.getParameters()[0]))
		{
			reply = arguments.getParameters()[0] + " " + arguments.getParameters()[1] + " :is already on channel \r\n";
			send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
		}
		if (!channel->clientOnChannel(client->getNickname()))
			return ERR_NOTONCHANNEL;
		if (channel->isInvited(arguments.getParameters()[0]) == false)
			channel->addInvited(server->findClientByNicknamme(arguments.getParameters()[0]));
	}
	reply = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " INVITE " + arguments.getParameters()[0] + " " + arguments.getParameters()[1] + "\r\n";
	send(server->findClientByNicknamme(arguments.getParameters()[0])->getPoll().fd, reply.c_str(), reply.size(), 0);
	reply = arguments.getParameters()[1] + " " + arguments.getParameters()[1];
	send(server->findClientByNicknamme(arguments.getParameters()[0])->getPoll().fd, reply.c_str(), reply.size(), 0);
	return 0;
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
	std::string reply;
	Server *server = arguments.getServer();
	Client *client = arguments.getClient();
	Channel *channel;
	if (arguments.getParameters().size() == 0)
		return ERR_NEEDMOREPARAMS;
	std::vector<std::string> channels = split(arguments.getParameters()[0], ",");
	std::vector<std::string> clients = split(arguments.getParameters()[1], ",");
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (!server->getChannel().count(*it))
		{
			reply = *it + " :No such channel\r\n";
			send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
			continue;
		}
		channel = server->getChannel()[*it];
		if (getMode().find("o") == std::string::npos && channel->getUserMode()[getPoll().fd].find("O") == std::string::npos && channel->getUserMode()[getPoll().fd].find("o") == std::string::npos)
			return ERR_CHANOPRIVSNEEDED;
		for (std::vector<std::string>::iterator cli = clients.begin(); cli != clients.end(); ++cli)
		{
			if (!channel->clientOnChannel(*cli))
			{
				reply = *cli + " " + channel->getName() + " :They aren't on that channel\r\n";
				send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
				continue;
			}
			std::map<int, Client *> users = channel->getClients();
			for (std::map<int, Client *>::iterator cli2 = users.begin() ; cli2 != users.end(); cli2++)
			{
				send(cli2->first, reply.c_str(), reply.size(), 0);
			}
			Client *bannedClient = server->findClientByNicknamme(*cli);
			channel->removeFromChannel(bannedClient, &arguments);
			bannedClient->leaveChannel(channel);
		}
	}
	return 0;
}
