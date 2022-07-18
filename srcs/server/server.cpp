/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 09:25:34 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/18 19:58:31 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"

Server::Server() : _name(), _fds()
{
	return;
}

Server::~Server()
{
	return;
}

// Server::Server(Server const & rhs)
// {
//     return ;
// }

// Server &Server::operator=(Server const & rhs)
// {
//     return _null;
// }

void Server::init()
{
	int				 	socketfd, on;
	socklen_t			addr_size;
	struct sockaddr_in	addr;
	int 				opt;

	on = 1;
	if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		(strerror(errno));
	if ((opt = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
		strerror(errno);
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6667);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(socketfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
		strerror(errno);
	if (listen(socketfd, 3) == -1)
		return;
	addr_size = sizeof(addr);
	_fds.fd = socketfd;
	_fds.events = POLLIN;
}

void Server::launch()
{
	struct sockaddr_storage		client_addr;
	socklen_t					addr_size;
	int							client_sock;
	std::vector<struct pollfd>	pollfds;
	struct pollfd				client;
	char						buffer[1025];
	std::string					message;
	for (;;)
	{
		pollfds.push_back(_fds);
		if (poll(&pollfds[0], pollfds.size(), -1) == -1)
			strerror(errno);
		;
		std::vector<pollfd>::iterator beg;
		for (beg = pollfds.begin(); beg != pollfds.end(); beg++)
		{
			if (beg->fd == _fds.fd)
			{
				std::cout << "Server :" << std::endl;
				addr_size = sizeof(client_addr);
				client_sock = accept(_fds.fd, (struct sockaddr *)&client_addr, &addr_size);
				if (client_sock == -1)
					strerror(errno);
				if (fcntl(client_sock, F_SETFL, O_NONBLOCK) == -1)
					strerror(errno);
				client.fd = client_sock;
				client.events = POLLIN | POLLHUP;
				client.revents = 0;
				pollfds.push_back(client);
			}
			else
			{
				client = pollfds[1];
				std::cout << "Client: ";
				recv(client_sock, buffer, 1024, 0);
				std::cout << buffer << " ";
			}
		}
	}
}