/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 09:25:34 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/22 19:31:08 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"
#


Server::Server() : _name("ircServer"), _fds(), _nbClients(0) 
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

std::string Server::getName()
{
	return _name;
}

std::map<std::string, Channel *> Server::getChannel()
{
	return _channel;
}

void Server::addChannel(Channel *channel)
{
	this->_channel.insert(std::pair<std::string, Channel *>(channel->getName(), channel));

}

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
	if (fcntl(socketfd, F_SETFL, O_NONBLOCK) < -1)
		strerror(errno);
	if (bind(socketfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1)
		strerror(errno);	
	if (listen(socketfd, NB_CLIENTS_MAX) == -1)
		return ;
	addr_size = sizeof(addr);
	_fds.fd = socketfd;
	_fds.events = POLLIN;
	_fds.revents = POLLIN;
	std::cout << "Welcome to IRCinator" << std::endl;
}

void Server::launch()
{

	std::vector<struct pollfd>		pollfds;
	Client 							*client;


	for (;;)
	{
		pollfds.push_back(_fds);
		if (poll(&pollfds[0], _nbClients + 1, -1) == -1)
			strerror(errno);
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			std::cout << "client tab fd : " << (*it).second->getPoll().fd << std::endl;
			pollfds.push_back((*it).second->getPoll());
		}
		std::cout << "fds fd : " << pollfds[0].revents << "revents " << POLLIN << std::endl;
		std::vector<pollfd>::iterator beg = pollfds.begin();
		std::vector<pollfd>::iterator end = pollfds.end();
		while (beg != end)
		{
			// if (beg->revents & POLLHUP && beg->fd != _fds.fd)
			// {
			// 	removeClient(beg->fd);
			// }
			if (beg->revents & POLLIN)
			{
				// for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
				// {
				// 	std::cout << "client status " << it->second->getStatus() << std::endl;
				// }
				if (beg->fd == _fds.fd)
				{
					std::cout << "server :" << std::endl;
					if (_nbClients < NB_CLIENTS_MAX )
						acceptClient();
					std::cout << "nb client : " << _nbClients << std::endl;
					std::cout << "fd : " << pollfds[0].fd << std::endl;
				}
				else
				{
					client = _clients[beg->fd];
					if (client == NULL)
						return ;
					client->treatMessage();
					// std::cout << "status 1:" << client->getStatus() << std::endl;
					if (client->getStatus() == DISCONNECTED)
						removeClient(client->getPoll().fd);
					else if (client->getStatus() == PENDING)
						rplWelcome(client);
					else if (client->getStatus() == CONNECTED)
					{
						std::vector<Command *> commands = client->getCommands();
						for (std::vector<Command *>::iterator it = commands.begin(); it != commands.end(); it++)
						{
							std::string reply;
							std::cout << "prefix " << (*it)->getPrefix() << std::endl;
							std::vector<std::string> params = (*it)->getParameters();
							for (size_t i = 0; i < params.size(); i++)
							{
								std::cout << "params : " << (*it)->getParameters()[i] << std::endl;
							}
							if ((*it)->getPrefix() == "NICK")
							{
								reply += ":" + client->getNickname() + "!" + client->getUsername() + "@localhost NICK " + (*it)->getParameters()[0] + "\r\n";
								std::string nick = (*it)->getParameters()[0];
								for (std::map<int, Client *>::iterator cli = _clients.begin(); cli != _clients.end(); cli++)
								{
									if (cli->second->getNickname() == nick)
										nick += "_";
								}
								client->setNickname(nick);
								send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
							}
							if ((*it)->getPrefix() == "JOIN")
							{
								JOIN(*it);
								std::map<std::string, Channel *> chann = getChannel();
								// for (std::map<std::string, Channel *>::iterator ite = chann.begin(); ite != chann.end(); ite++)
								// {
								// 	std::cout << "channels : " << (*ite).first << std::endl;
								// 	// std::cout << "channels client : " << (*ite).second->getClients()[client->getPoll().fd]->getNickname() << std::endl;
								// }
							}
						}
					}
					client->clearCommands();
					std::cout << "fd : " << client->getPoll().fd << std::endl;
	
				}
			}
				// else{
				// 	std::cout << "else" << std::endl;
				// }
			beg++;
		}
		pollfds.clear();
		std::cout << "size after clear : " << pollfds.size() << std::endl;
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
	fds.revents = POLLIN;
	this->_clients.insert(std::pair<int, Client *>(client_sock, new Client(fds, this)));
	this->_nbClients += 1;
}

void Server::removeClient(int fd)
{
	close(fd);
	this->_clients.erase(fd);
	this->_nbClients -= 1;
}

bool Server::nickNameUsed(std::string nickname)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return true;
	}
	return false;
}

void Server::rplWelcome(Client *client)
{
	std::string reply;
	std::string reply2;
	std::cout << "********REGISTRATION SUCCESS for " << client->getNickname() << "**********" << std::endl;		
	std::cout << "fd registred : " << client->getPoll().fd << std::endl;

	client->setStatus(CONNECTED);
	// _nbClients++;
	reply += ":" + _name + " 001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname() + "\r\n";
	reply2 +=  ":" + _name + " 376 " + client->getNickname() + " :End of /MOTD command\r\n";
	send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	send(client->getPoll().fd, reply2.c_str(), reply2.size(), 0);
}

void Server::changeClientClass(Client oldClient, Client *newClient)
{
	t_pollfd newPoll;
	newPoll = oldClient.getPoll();
	newClient->setPoll(newPoll);
	_clients[oldClient.getPoll().fd] = newClient; 
}