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
#include <string>
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

class server
{
public:
    server();// meant to launch the server
    server(server const & rhs); // lauch another server
    ~server();// destroys all channel and everything that was set
    server &operator=(server const & rhs);

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
	int				listenning_socket;
	sockaddr_in		hint_address;


	/* Connection initialisation */
	int initialise_socket();
	int bind_socket();
	int init_descriptors();
	int await_activity();
	int handle_incomming_connection();
	int handle_other_connection();



	/* Error types */

};

#endif