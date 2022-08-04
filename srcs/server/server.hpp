/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamanfo <kamanfo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 11:02:06 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/08/03 21:41:35 by kamanfo          ###   ########.fr       */
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
#include <signal.h>
#include "../client/client.hpp"
#include "../channel/channel.hpp"

#define NB_CLIENTS_MAX 5

class Client;

class Server
{
public:
    Server();// meant to launch the server
    Server(std::string port, std::string password);// meant to launch the server
    Server(Server const & rhs); // lauch another server
    ~Server();// destroys all channel and everything that was set
	//server &operator=(server const & rhs);

	void init();
	void launch();


	/* GETTERS */

	std::string getName() const;
	std::map<int, Client *> getClients() const;
	std::map<std::string, Channel *> getChannel() const;
	std::string 	getVersion() const;
	std::string 	getPassword() const;

	/* Handle incomming strings */
	// in charge of calling the right function

	/* channel operation */
	void destroyChannel(Channel *channel);

	/* user opération */
	void acceptClient();
	void removeClient(int fd);
	void changeClientClass(Client *oldClient, Client *newClient);
	
	// COMMAND RESPONSE
	void rplWelcome(Client *client);

	/* utils for client */
	bool nickNameUsed(std::string nickname);
	bool correctPassword(std::string password);
	Client *findClientByNicknamme(std::string nickname);

	/* channel operation */
	void addChannel(Channel *channel);

	/* DISPLAY */
	void displayServer();

	/* handle signal */

private:
	std::string	_port;
	std::string	_password;
	std::string _name;
	std::string _hostname;
	std::map<std::string, Channel *> _channel;
	typedef struct pollfd	t_pollfd;
	t_pollfd				_fds;
	std::map<int, Client *> _clients;
	unsigned int _nbClients;
	std::string _version;

	/* Error types */
	std::vector<Client *>	_toDelClient;
};

#endif
