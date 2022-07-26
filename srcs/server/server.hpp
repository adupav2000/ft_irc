/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 11:02:06 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/26 16:49:41 by AlainduPa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#define PORT 6667
#define TRUE 1

#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <poll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <string.h>    // for strncmp
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include "../client/client.hpp"
#include "../channel/channel.hpp"

#define NB_CLIENTS_MAX 5

class Client;

class Server
{
public:
    Server();// meant to launch the server
    Server(Server const & rhs); // lauch another server
    ~Server();// destroys all channel and everything that was set
	//server &operator=(server const & rhs);

	void init();
	void launch();


	/* GETTERS */

	std::string getName() const;
	std::map<int, Client *> getClients();
	std::map<std::string, Channel *> getChannel();

	/* Handle incomming strings */
	// in charge of calling the right function

	/* channel operation */
	int destroyChannel();

	/* user opération */
	void acceptClient();
	void removeClient(int fd);
	void changeClientClass(Client *oldClient, Client *newClient);
	
	// COMMAND RESPONSE

	void rplWelcome(Client *client);

	/* utils for client */
	bool nickNameUsed(std::string nickname);

	/* channel operation */
	std::map<std::string, Channel *> getChannel();
	void addChannel(Channel *channel);

private:
	std::string _name;
	std::map<std::string, Channel *> _channel;

	typedef struct pollfd	t_pollfd;
	t_pollfd				_fds;
	std::map<int, Client *> _clients;
	unsigned int _nbClients;


	/* Error types */
	
};

#endif
