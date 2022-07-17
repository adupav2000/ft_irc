/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 10:21:57 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/17 18:18:49 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

server::server()
{
	
}

server::~server()
{

}

server::server(server const & rhs)
{
	(void)rhs;
}
/*
server &server::operator=(server const & rhs)
{
	(void)rhs;
	return (this);
}
*/

void server::init()
{
	this->initialise_socket();
	this->bind_socket();
	this->init_descriptors();
	this->await_activity();
	this->handle_incomming_connection();
	this->handle_other_connection();
}

void server::initialise_socket()
{
	_max_clients = 30;
	_message = "Message test";
	for (_i = 0; _i < _max_clients; _i++)
		_client_socket[_i] = 0;
	// create a master socket
	if ((_master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "socket failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	// set master socket to allow multiple connections ,
	// this is just a good habit, it will work without this
	if (setsockopt(_master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&_opt,
				   sizeof(_opt)) < 0)
	{
		std::cerr << "setsockopt" << std::endl;
		exit(EXIT_FAILURE);
	}

	// type of socket created
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(PORT);
}

void server::bind_socket()
{
	// bind the socket to localhost port 8888
	if (bind(_master_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listener on port " << PORT << std::endl;
	// try to specify maximum of 3 pending connections for the master socket
	if (listen(_master_socket, 3) < 0)
	{
		std::cerr << "Listen" << std::endl;
		exit(EXIT_FAILURE);
	}

	// accept the incoming connection
	_addrlen = sizeof(_address);
	puts("Waiting for connections ...");
}

void server::init_descriptors()
{
	while (TRUE)	
	{
		this->await_activity();
		this->handle_incomming_connection();
		this->handle_other_connection();
	}

}

void server::await_activity()
{
		// clear the socket set
		FD_ZERO(&_readfds);

		// add master socket to set
		FD_SET(_master_socket, &_readfds);
		_max_sd = _master_socket;

		// add child sockets to set
		for (_i = 0; _i < _max_clients; _i++)
		{
			// socket descriptor
			_sd = _client_socket[_i];

			// if valid socket descriptor then add to read list
			if (_sd > 0)
				FD_SET(_sd, &_readfds);

			// highest file descriptor number, need it for the select function
			if (_sd > _max_sd)
				_max_sd = _sd;
		}
}

void server::handle_incomming_connection()
{
	// wait for an activity on one of the sockets , timeout is NULL ,
	// so wait indefinitely
	_activity = select(_max_sd + 1, &_readfds, NULL, NULL, NULL);

	if ((_activity < 0) && (errno != EINTR))
	{
		std::cout << "select error " << std::endl;
	}

	// If something happened on the master socket ,
	// then its an incoming connection
	if (FD_ISSET(_master_socket, &_readfds))
	{
		if ((_new_socket = accept(_master_socket,
									(struct sockaddr *)&_address, (socklen_t *)&_addrlen)) < 0)
		{
			std::cerr << "Accept" << std::endl;
			exit(EXIT_FAILURE);
		}

		// inform user of socket number - used in send and receive commands
		std::cout << "New connection , socket fd is " << _new_socket << " , ip is : " << inet_ntoa(_address.sin_addr) << ", port : " << ntohs(_address.sin_port) << std::endl;
		// send new connection greeting message
		if (send(_new_socket, (_message), strlen(static_cast<const char *>(_message)), 0) != (ssize_t)strlen(static_cast<const char *>(_message)))
		{
			std::cout << "send" << std::endl;
		}

		puts("Welcome message sent successfully");

		// add new socket to array of sockets
		for (_i = 0; _i < _max_clients; _i++)
		{
			// if position is empty
			if (_client_socket[_i] == 0)
			{
				_client_socket[_i] = _new_socket;
				std::cout << "Adding to list of sockets as " << _i << std::endl;
				break;
			}
		}
	}
}

void server::handle_other_connection()
{
	std::cout << "Here it breaks" << std::endl;
	for (_i = 0; _i < _max_clients; _i++)
	{
		std::cout << "Here it breaks"  << _i << std::endl;
		_sd = _client_socket[_i];

		if (FD_ISSET(_sd, &_readfds))
		{
			// Check if it was for closing , and also read the
			// incoming message
			if ((_valRead = read(_sd, _buffer, 1024)) == 0)
			{
				// Somebody disconnected , get his details and print
				getpeername(_sd, (struct sockaddr *)&_address,
							(socklen_t *)&_addrlen);
				std::cout << "Host disconnected , ip " << inet_ntoa(_address.sin_addr) << " , port " << ntohs(_address.sin_port) << std::endl;

				// Close the socket and mark as 0 in list for reuse
				std::cout << "3Here it breaks"  << _i << std::endl;
				close(_sd);
				_client_socket[_i] = 0;
			}
			// Echo back the message that came in
			else
			{
				// set the string terminating NULL byte on the end
				// of the data read

				_buffer[_valRead] = '\0';

				// 	std::cout << "4Here it breaks"  << _i << std::endl;
				// 	try
				// 	{
				// 		send(_sd, _buffer, strlen(_buffer), 0);
				// 		/* code */
				// 	}
				// 	catch(const std::exception& e)
				// 	{
				// 		std::cerr << "Send function failing : " <<  e.what() << '\n';
				// 	}
			}
		}
	}
}
