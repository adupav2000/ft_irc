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
#define PORT 66667
#define TRUE 1
#include <string>
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

class Server
{
public:
    Server();// meant to launch the Server
    Server(Server const & rhs); // lauch another Server
    ~Server();// destroys all channel and everything that was set
	//Server &operator=(Server const & rhs);

	void init();

	/* Handle incomming strings */
	// in charge of calling the right function

	/* channel operation */
	int create_channel();
	int destroy_channel();

private:
	std::string name;
	//Client map<int, std::string> client_list;
	//Service map<int, std::string> service_list;
	/* client operation */
	int new_client(); // sends a PL_WELCOME and adds a client in map
	int new_service(); // sends a RPL_YOURESERVICE and adds a service in maps

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