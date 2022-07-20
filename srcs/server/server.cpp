/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 09:25:34 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/20 15:56:48 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"
#


Server::Server() : _name("ircServer"), _fds()
{
    return ;
}

Server::~Server()
{
    return ;
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
    int					socketfd, on;
	struct sockaddr_in	addr;
	socklen_t			addr_size;

	on = 1;
	if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		(strerror(errno)) ;
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR , &on, sizeof(on)) < 0)
		strerror(errno);
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6667);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (fcntl(socketfd, F_SETFL, O_NONBLOCK) <-1)
		strerror(errno);
	if (bind(socketfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1)
		strerror(errno);	
	if (listen(socketfd, 3) == -1)
		return ;
	addr_size = sizeof(addr);
	_fds.fd = socketfd;
	_fds.events = POLLIN;
	std::cout << "POLLIN" << POLLIN << std::endl;
	std::cout << "POLLHUP" << POLLHUP << std::endl;
}

void Server::launch()
{

	std::vector<struct pollfd>		pollfds;
	struct pollfd					client;

	for (;;)
	{
		pollfds.push_back(_fds);
		if (poll(&pollfds[0], pollfds.size() + 1, -1) == -1)
			strerror(errno);
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			pollfds.push_back((*it).second->getPoll());
		}
		std::vector<pollfd>::iterator beg = pollfds.begin();
		std::vector<pollfd>::iterator end = pollfds.end();
		while (beg != end)
		{
			if (beg->revents & POLLIN)
			{
				if (beg->fd == _fds.fd)
				{
					std::cout << "server :" << std::endl;
					acceptClient();
					std::cout << "nb client : " << pollfds.size() << std::endl;
				}
				else {
					client = _clients[beg->fd]->getPoll(); 
					_clients[beg->fd]->treatMessage();
				}
			}
			beg++;
		}
		pollfds.clear();
	}
}

void Server::acceptClient()
{
	int								client_sock;
	struct sockaddr_storage 		client_addr;
	socklen_t						addr_size;
	struct pollfd					fds;

	addr_size = sizeof(client_addr);
	client_sock = accept(_fds.fd, (struct sockaddr *)&client_addr, &addr_size);
	if (client_sock == -1)
		strerror(errno);
	if (fcntl(client_sock, F_SETFL,  O_NONBLOCK) == -1)
		strerror(errno);
		
	fds.fd = client_sock;
	fds.events = POLLIN;
	this->_clients.insert(std::pair<int, Client *>(client_sock, new Client(fds)));
	this->_nb_clients += 1;
}

void Server::removeClient(int fd)
{
	this->_clients.erase(fd);
	this->_nb_clients -= 1;
}

bool Server::searchNickname(std::string nickname)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return true;
	}
	return false;
}

// void Server::rplWelcome(Client client)
// {
// 	return ;
// }