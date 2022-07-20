/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:29 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/18 15:07:46 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
#include <iostream>
#include <map>
#include <utility>

#include "../server/server.hpp"
#include "../globals.hpp"
#define BUFFER_SIZE 1024

enum clientStatus {

};

class Client
{
public:
    Client();
    Client(struct pollfd poll);
    Client(Client const & rhs);
    ~Client();
    Client &operator=(Client const & rhs);
	// Server &_serverRef;
	int execCommand(std::string arguments);
	
	// GETTERS

	struct pollfd const &getPoll();
	std::string const &getNickname();


	void treatMessage();
	
protected:
 
	/* Variables */
	struct pollfd _fds;
	std::string _nickname;

	typedef std::map<std::string, int (*)(std::string)> t_messFuncMap;
	typedef std::pair<std::string, int (*)(std::string)> t_messFuncPair;
	// t_messFuncMap	_messageFunctions;
	std::map<std::string, int (*)(std::string)> _messageFunctions;



	/* Connection registration functions*/
	int NICK(std::string arguments);
	int USER(std::string arguments);
	int MODE(std::string arguments);
	int SERVICE(std::string arguments);
	int QUIT(std::string arguments);
	int SQUIT(std::string arguments);

	/* Channel operations */
	int JOIN(std::string arguments);
	int PART(std::string arguments);
	
	/* Utils */
	int isDigit(char c);
	int isLetter(char c);
	int isSpecial(char c);

};

#endif