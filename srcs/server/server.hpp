/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 11:02:06 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/14 10:20:57by adu-pavi         ###   ########.fr       */
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

	/* Handle incomming strings */
	// in charge of calling the right function

	/* channel operation */
	int createChannel();
	int destroyChannel();

	/* user opération */
	void acceptClient();
	void removeClient(int fd);
	
	// COMMAND RESPONSE

	void rplWelcome(Client client);


private:
	std::string _name;

	typedef struct pollfd	t_pollfd;
	t_pollfd				_fds;
	std::map<int, Client *> _clients;
	unsigned int _nb_clients;

	//Service map<int, std::string> service_list;
	/* client operation */
	int new_client(); // sends a PL_WELCOME and adds a client in map
	int new_service(); // sends a RPL_YOURESERVICE and adds a service in maps
	bool searchNickname(std::string nickname);

	/* A list of connection requirement variables */
	int				_listenning_socket;
	int				_opt;
	sockaddr_in		_hint_address;
	int				_master_socket;
	int				_addrlen;
	int 			_new_socket;
	int				_client_socket[30];
	int				_max_clients;
	int				_activity;
	int				_i;
	int 			_valRead;
	int 			_sd;
	int				_max_sd;
	struct sockaddr_in	_address;
	char			_buffer[1025];
	fd_set 			_readfds;
	const void 		*_message;


	/* Connection initialisation */
	void initialise_socket();
	void bind_socket();
	void init_descriptors();
	void await_activity();
	void handle_incomming_connection();
	void handle_other_connection();

	/* Error types */
	
};

#endif