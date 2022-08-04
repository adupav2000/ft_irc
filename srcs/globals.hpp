/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamanfo <kamanfo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:37:01 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/08/04 16:29:57 by kamanfo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <iostream>
#include <string.h>
#include <ctime>

std::vector<std::string> split(std::string str, std::string delimiter);

#define ERR_ALREADYREGISTRED 462 
       /*       ":Unauthorized command (already registered)"


         - Returned by the server to any link which tries to
           change part of the registered details (such as
           password or user details from second USER message).
*/
#define ERR_NONICKNAMEGIVEN 431 
/*              ":No nickname given"
         - Returned when a nickname parameter expected for a
           command and isn't found.
*/

#define ERR_RESTRICTED 484
              /*
              ":Your connection is restricted!"

         - Sent by the server to a user upon connection to indicate
           the restricted nature of the connection (user mode "+r").
		   */
#define RPL_YOURESERVICE 383 
#define ERR_NOTEXTTOSEND 412
#define RPL_INFO 371
#define RPL_ENDOFINFO 374
#define RPL_ENDOFWHOIS 318    
#define RPL_MOTDSTART 375    

#define RPL_VERSION 351
#define ERR_ALREADYREGISTRED 462
#define RPL_YOUREOPER 381
#define ERR_ALREADYREGISTRED 462

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

#define ERR_UNKNOWNERROR 400 
#define ERR_NOSUCHNICK 401 
#define ERR_NOSUCHSERVER 402 
#define ERR_NOSUCHCHANNEL 403 
#define ERR_CANNOTSENDTOCHAN 404 
#define ERR_TOOMANYCHANNELS 405 
#define ERR_WASNOSUCHNICK 406 
#define ERR_NOORIGIN 409 
#define ERR_INPUTTOOLONG 417 
#define ERR_UNKNOWNCOMMAND 421 
#define ERR_NOMOTD 422 
#define ERR_ERRONEUSNICKNAME 432 
#define ERR_NICKNAMEINUSE 433 
#define ERR_USERNOTINCHANNEL 441 
#define ERR_NOTONCHANNEL 442 
#define ERR_USERONCHANNEL 443 
#define ERR_NOTREGISTERED 451 
#define ERR_NEEDMOREPARAMS 461 
#define ERR_ALREADYREGISTERED 462 
#define ERR_PASSWDMISMATCH 464 
#define ERR_YOUREBANNEDCREEP 465 
#define ERR_CHANNELISFULL 471 
#define ERR_UNKNOWNMODE 472 
#define ERR_INVITEONLYCHAN 473 
#define ERR_BANNEDFROMCHAN 474 
#define ERR_BADCHANNELKEY 475 
#define ERR_BADCHANMASK 476 
#define ERR_NOPRIVILEGES 481 
#define ERR_CHANOPRIVSNEEDED 482 
#define ERR_CANTKILLSERVER 483 
#define ERR_NOOPERHOST 491 
#define ERR_UMODEUNKNOWNFLAG 501 
#define ERR_USERSDONTMATCH 502 
#define ERR_HELPNOTFOUND 524 
#define ERR_INVALIDKEY 525 
#define RPL_STARTTLS 670 
#define RPL_WHOISSECURE 671 
#define ERR_STARTTLS 691 
#define ERR_INVALIDMODEPARAM 696 
#define RPL_HELPSTART 704 
#define RPL_HELPTXT 705 
#define RPL_ENDOFHELP 706 
#define ERR_NOPRIVS 723 
#define RPL_LOGGEDIN 900 
#define RPL_LOGGEDOUT 901 
#define ERR_NICKLOCKED 902 
#define RPL_SASLSUCCESS 903 
#define ERR_SASLFAIL 904 
#define ERR_SASLTOOLONG 905 
#define ERR_SASLABORTED 906 
#define ERR_SASLALREADY 907 
#define RPL_SASLMECHS 908 


#define SEND_CONFIRMNEWNICK 1000
	/* confirms a new nickname changes */

#define SEND_CONFIRMDISCONNECT 1001

#endif

