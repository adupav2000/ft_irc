/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:37:01 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/21 17:49:20 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>

#define ERR_NOSUCHNICK 401 
#define MESS_ERR_NOSUCHNICK "<nickname> :No such nick/channel"
/*              "<nickname> :No such nick/channel"

          - Used to indicate the nickname parameter supplied to a
            command is currently unused.
*/

#define ERR_NOSUCHSERVER 402 
#define MESS_ERR_NOSUCHSERVER "<server name> :No such server"
/*              "<server name> :No such server"

         - Used to indicate the server name given currently
           does not exist.
*/

#define ERR_NOSUCHCHANNEL 403 
#define MESS_ERR_NOSUCHCHANNEL "<channel name> :No such channel"
/*              "<channel name> :No such channel"

         - Used to indicate the given channel name is invalid.
*/

#define ERR_CANNOTSENDTOCHAN 404 
#define MESS_ERR_CANNOTSENDTOCHAN "<channel name> :Cannot send to channel"
/*              "<channel name> :Cannot send to channel"

         - Sent to a user who is either (a) not on a channel
           which is mode +n or (b) not a chanop (or mode +v) on
           a channel which has mode +m set or where the user is
           banned and is trying to send a PRIVMSG message to
           that channel.
*/

#define ERR_TOOMANYCHANNELS 405 
#define MESS_ERR_TOOMANYCHANNELS "<channel name> :You have joined too many channels"
/*              "<channel name> :You have joined too many channels"

         - Sent to a user when they have joined the maximum
           number of allowed channels and they try to join
           another channel.
*/

#define ERR_WASNOSUCHNICK 406 
#define MESS_ERR_WASNOSUCHNICK "<nickname> :There was no such nickname"
/*              "<nickname> :There was no such nickname"

         - Returned by WHOWAS to indicate there is no history
           information for that nickname.
*/

#define ERR_TOOMANYTARGETS 407 
#define MESS_ERR_TOOMANYTARGETS "<target> :<error code> recipients. <abort message>"
/*              "<target> :<error code> recipients. <abort message>"

         - Returned to a client which is attempting to send a
           PRIVMSG/NOTICE using the user@host destination format
           and for a user@host which has several occurrences.

         - Returned to a client which trying to send a
           PRIVMSG/NOTICE to too many recipients.

         - Returned to a client which is attempting to JOIN a safe
           channel using the shortname when there are more than one
           such channel.
*/

#define ERR_NOSUCHSERVICE 408 
#define MESS_ERR_NOSUCHSERVICE "<service name> :No such service"
/*              "<service name> :No such service"

         - Returned to a client which is attempting to send a SQUERY
           to a service which does not exist.
*/

#define ERR_NOORIGIN 409 
#define MESS_ERR_NOORIGIN ":No origin specified"
/*              ":No origin specified"

         - PING or PONG message missing the originator parameter.
*/

#define ERR_NORECIPIENT 411 
#define MESS_ERR_NORECIPIENT ":No recipient given (<command>)"
/*              ":No recipient given (<command>)"*/
#define ERR_NOTEXTTOSEND 412 
#define MESS_ERR_NOTEXTTOSEND ":No text to send"
/*              ":No text to send"*/
#define ERR_NOTOPLEVEL 413 
#define MESS_ERR_NOTOPLEVEL "<mask> :No toplevel domain specified"
/*              "<mask> :No toplevel domain specified"*/
#define ERR_WILDTOPLEVEL 414 
#define MESS_ERR_WILDTOPLEVEL "<mask> :Wildcard in toplevel domain"
/*              "<mask> :Wildcard in toplevel domain"*/
#define ERR_BADMASK 415 
#define MESS_ERR_BADMASK "<mask> :Bad Server/host mask"
/*              "<mask> :Bad Server/host mask"
         --are returned by PRIVMSG to indi that 412 
          the message wasn't delivered for some reason.
           ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that
           are returned when an invalid use of
           "PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.
*/

#define ERR_UNKNOWNCOMMAND 415 
#define MESS_ERR_UNKNOWNCOMMAND "<command> :Unknown command"
/*              "<command> :Unknown command"

         - Returned to a registered client to indicate that the
           command sent is unknown by the server.
*/
#define ERR_NOMOTD 421 
#define MESS_ERR_NOMOTD ":MOTD File is missing"
/*              ":MOTD File is missing"

         - Server's MOTD file could not be opened by the server.
*/
#define ERR_NOADMININFO 422 
#define MESS_ERR_NOADMININFO "<server> :No administrative info available"
/*              "<server> :No administrative info available"

         - Returned by a server in response to an ADMIN message
           when there is an error in finding the appropriate
           information.
*/
#define ERR_FILEERROR 423 
#define MESS_ERR_FILEERROR ":File error doing <file op> on <file>"
/*              ":File error doing <file op> on <file>"

         - Generic error message used to report a failed file
           operation during the processing of a message.
*/
#define ERR_NONICKNAMEGIVEN 424 
#define MESS_ERR_NONICKNAMEGIVEN ":No nickname given"
/*              ":No nickname given"

         - Returned when a nickname parameter expected for a
           command and isn't found.
*/
#define ERR_ERRONEUSNICKNAME 431 
#define MESS_ERR_ERRONEUSNICKNAME "<nick> :Erroneous nickname"
/*              "<nick> :Erroneous nickname"

         - Returned after receiving a NICK message which contains
           characters which do not fall in the defined set.  See
           section 2.3.1 for details on valid nicknames.*/

#define ERR_NICKNAMEINUSE 432 
#define MESS_ERR_NICKNAMEINUSE "<nick> :Nickname is already in use"
/*              "<nick> :Nickname is already in use"

         - Returned when a NICK message is processed that results
           in an attempt to change to a currently existing
           nickname.
*/

#define ERR_NICKCOLLISION 433 
#define MESS_ERR_NICKCOLLISION "<nick> :Nickname collision KILL from <user>@<host>"
/*              "<nick> :Nickname collision KILL from <user>@<host>"

         - Returned by a server to a client when it detects a
           nickname collision (registered of a NICK that
           already exists by another server).
*/

#define ERR_UNAVAILRESOURCE 436 
#define MESS_ERR_UNAVAILRESOURCE "<nick/channel> :Nick/channel is temporarily unavailable"
/*              "<nick/channel> :Nick/channel is temporarily unavailable"

         - Returned by a server to a user trying to join a channel
           currently blocked by the channel delay mechanism.
         - Returned by a server to a user trying to change nickname
           when the desired nickname is blocked by the nick delay
           mechanism.
*/

#define ERR_USERNOTINCHANNEL 437 
#define MESS_ERR_USERNOTINCHANNEL "<nick> <channel> :They aren't on that channel"
/*              "<nick> <channel> :They aren't on that channel"

         - Returned by the server to indicate that the target
           user of the command is not on the given channel.
*/

#define ERR_NOTONCHANNEL 441 
#define MESS_ERR_NOTONCHANNEL "<channel> :You're not on that channel"
/*              "<channel> :You're not on that channel"

         - Returned by the server whenever a client tries to
           perform a channel affecting command for which the
           client isn't a member.
*/

#define ERR_USERONCHANNEL 442 
#define MESS_ERR_USERONCHANNEL "<user> <channel> :is already on channel"
/*              "<user> <channel> :is already on channel"

         - Returned when a client tries to invite a user to a
           channel they are already on.
*/

#define ERR_NOLOGIN 443 
#define MESS_ERR_NOLOGIN "<user> :User not logged in"
/*              "<user> :User not logged in"

         - Returned by the summon after a SUMMON command for a
           user was unable to be performed since they were not
           logged in.
*/

#define ERR_SUMMONDISABLED 444 
#define MESS_ERR_SUMMONDISABLED ":SUMMON has been disabled"
/*              ":SUMMON has been disabled"

         - Returned as a response to the SUMMON command.  MUST be
           returned by any server which doesn't implement it.*/

#define ERR_USERSDISABLED 445 
#define MESS_ERR_USERSDISABLED ":USERS has been disabled"
/*              ":USERS has been disabled"

         - Returned as a response to the USERS command.  MUST be
           returned by any server which does not implement it.
*/
#define ERR_NOTREGISTERED 446 
#define MESS_ERR_NOTREGISTERED ":You have not registered"
/*              ":You have not registered"

         - Returned by the server to indicate that the client
           MUST be registered before the server will allow it
           to be parsed in detail.
*/
#define ERR_NEEDMOREPARAMS 451 
#define MESS_ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
/*              "<command> :Not enough parameters"

         - Returned by the server by numerous commands to
           indicate to the client that it didn't supply enough
           parameters.
*/

#define ERR_ALREADYREGISTRED 461 
#define MESS_ERR_ALREADYREGISTRED ":Unauthorized command (already registered)"
/*              ":Unauthorized command (already registered)"

         - Returned by the server to any link which tries to
           change part of the registered details (such as
           password or user details from second USER message).
*/

#define ERR_NOPERMFORHOST 462 
#define MESS_ERR_NOPERMFORHOST ":Your host isn't among the privileged"
/*              ":Your host isn't among the privileged"

         - Returned to a client which attempts to register with
           a server which does not been setup to allow
           connections from the host the attempted connection
           is tried.
*/

#define ERR_PASSWDMISMATCH 463 
#define MESS_ERR_PASSWDMISMATCH ":Password incorrect"
/*              ":Password incorrect"

         - Returned to indicate a failed attempt at registering
           a connection for which a password was required and
           was either not given or incorrect.
*/

#define ERR_YOUREBANNEDCREEP 464 
#define MESS_ERR_YOUREBANNEDCREEP ":You are banned from this server"
/*              ":You are banned from this server"

         - Returned after an attempt to connect and register
           yourself with a server which has been setup to
           explicitly deny connections to you.
*/

#define ERR_YOUWILLBEBANNED 465 
/*
         - Sent by a server to a user to inform that access to the
           server will soon be denied.
*/

#define ERR_KEYSET 466 
#define MESS_ERR_KEYSET               "<channel> :Channel key already set"
/*              "<channel> :Channel key already set"*/

#define ERR_CHANNELISFULL 467 
#define MESS_ERR_CHANNELISFULL               "<channel> :Cannot join channel (+l)"
/*              "<channel> :Cannot join channel (+l)"*/

#define ERR_UNKNOWNMODE 471 
#define MESS_ERR_UNKNOWNMODE               "<char> :is unknown mode char to me for <channel>"
/*              "<char> :is unknown mode char to me for <channel>"*/

#define ERR_INVITEONLYCHAN 472 
#define MESS_ERR_INVITEONLYCHAN               "<channel> :Cannot join channel (+i)"
/*              "<channel> :Cannot join channel (+i)"*/

#define ERR_BANNEDFROMCHAN 473 
#define MESS_ERR_BANNEDFROMCHAN               "<channel> :Cannot join channel (+b)"
/*              "<channel> :Cannot join channel (+b)"*/

#define ERR_BADCHANNELKEY 474 
#define MESS_ERR_BADCHANNELKEY               "<channel> :Cannot join channel (+k)"
/*              "<channel> :Cannot join channel (+k)"*/

#define ERR_BADCHANMASK 475 
#define MESS_ERR_BADCHANMASK               "<channel> :Bad Channel Mask"
/*              "<channel> :Bad Channel Mask"*/

#define ERR_NOCHANMODES 476 
#define MESS_ERR_NOCHANMODES               "<channel> :Channel doesn't support modes"
/*              "<channel> :Channel doesn't support modes"*/

#define ERR_BANLISTFULL 477 
#define MESS_ERR_BANLISTFULL               "<channel> <char> :Channel list is full"
/*              "<channel> <char> :Channel list is full"*/

#define ERR_NOPRIVILEGES 478 
#define MESS_ERR_NOPRIVILEGES               ":Permission Denied- You're not an IRC operator"
/*              ":Permission Denied- You're not an IRC operator"

         - Any command requiring operator privileges to operate
           MUST return this error to indicate the attempt was
           unsuccessful.
*/

#define ERR_CHANOPRIVSNEEDED 481 
#define MESS_ERR_CHANOPRIVSNEEDED               "<channel> :You're not channel operator"
/*              "<channel> :You're not channel operator"

         - Any command requiring 'chanop' privileges (such as
           MODE messages) MUST return this error if the client
           making the attempt is not a chanop on the specified
           channel.
*/

#define ERR_CANTKILLSERVER 482 
#define MESS_ERR_CANTKILLSERVER               ":You can't kill a server"
/*              ":You can't kill a server!"

         - Any attempts to use the KILL command on a server
           are to be refused and this error returned directly
           to the client.
*/

#define ERR_RESTRICTED 483 
#define MESS_ERR_RESTRICTED               ":Your connection is restricted"
/*              ":Your connection is restricted!"

         - Sent by the server to a user upon connection to indicate
           the restricted nature of the connection (user mode "+r").
*/

#define ERR_UNIQOPPRIVSNEEDED 484 
#define MESS_ERR_UNIQOPPRIVSNEEDED               ":You're not the original channel operator"
/*              ":You're not the original channel operator"

         - Any MODE requiring "channel creator" privileges MUST
           return this error if the client making the attempt is not
           a chanop on the specified channel.
*/

#define ERR_NOOPERHOST 485 
#define MESS_ERR_NOOPERHOST               ":No O-lines for your host"
/*              ":No O-lines for your host"

         - If a client sends an OPER message and the server has
           not been configured to allow connections from the
           client's host as an operator, this error MUST be
           returned.
*/

#define ERR_UMODEUNKNOWNFLAG 501 
#define MESS_ERR_UMODEUNKNOWNFLAG ":Unknown MODE flag"
/*
":Unknown MODE flag"

- Returned by the server to indicate that a MODE
message was sent with a nickname parameter and that
the a mode flag sent was not recognized.
*/

#define ERR_USERSDONTMATCH 502 
#define MESS_ERR_USERSDONTMATCH ":Cannot change mode for other users"
/*    
":Cannot change mode for other users"

- Error sent to any user trying to view or change the
user mode for a user other than themselves.
*/

/* user modes */
#define AWAY 'a'
 /* a - user is flagged as away; */

#define INVISIBLE 'i'
/*	i - marks a users as invisible;*/

#define REC_WALLOPS 'w'
/*	w - user receives wallops;*/

#define RESTRICTED 'r'
/*	r - restricted user connection;*/

#define G_OPERATOR 'o'
/*	o - operator flag;*/

#define LOC_OPERATOR 'O'
/*	O - local operator flag;*/

#define REC_USER_NOTICE 's'
/*	s - marks a user for receipt of server notices.*/

std::vector<std::string> split(std::string str, std::string delimiter);
