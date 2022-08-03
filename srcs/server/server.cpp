/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kamanfo <kamanfo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 09:25:34 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/08/03 22:54:01 by kamanfo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server.hpp"

int exitRequest = 0;

Server::Server (std::string port, std::string password) : _port(port), _password(password),  _name("ircServer"), _fds(), _nbClients(0), _version("1.0")
{
    return ;
}

Server::~Server()
{
	close(_fds.fd);
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		close(it->first);
		delete it->second;
	}
	_clients.clear();
	for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); it++)
		delete it->second;
	_channel.clear();
	std::cout << "fdsfsdfsd : " << std::endl;
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

void handler(int)
{
	exitRequest = 1;
}

std::string Server::getName() const 
{
	return _name;
}

std::map<std::string, Channel *> Server::getChannel() const 
{
	return _channel;
}

std::map<int, Client *> Server::getClients() const 
{
	return _clients;
}

std::string 	Server::getVersion() const
{
	return _version;
}

std::string 	Server::getPassword() const
{
	return _password;
}

void Server::addChannel(Channel *channel)
{
	this->_channel.insert(std::pair<std::string, Channel *>(channel->getName(), channel));

}

void Server::destroyChannel(Channel *channel)
{
	_channel.erase(channel->getName());
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
	addr.sin_port = htons(atoi(_port.c_str()));
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
	
	signal(SIGINT, handler);
	for (; !exitRequest ;)
	{
		pollfds.push_back(_fds);
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
			pollfds.push_back((*it).second->getPoll());
		if (poll(&pollfds[0], _nbClients + 1, -1) == -1)
			strerror(errno);
		std::vector<pollfd>::iterator beg = pollfds.begin();
		std::vector<pollfd>::iterator end = pollfds.end();
		while (beg != end)
		{
			if (beg->revents & POLLIN)
			{
				if (beg->fd == _fds.fd)
				{
					if (_nbClients < NB_CLIENTS_MAX )
						acceptClient();
				}
				else
				{
					client = _clients[beg->fd];
					if (client == NULL)
						return ;
					client->treatMessage();
					if (client->getStatus() == DISCONNECTED)
						removeClient(client->getPoll().fd);
					else if (client->getStatus() == PENDING)
						rplWelcome(client);
					else if (client->getStatus() == CONNECTED)
					{
						client->executeCommands();
					}
				}
			}
			beg++;
		}
		pollfds.clear();
		displayServer();
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
	std::string display;
	display = _clients[fd]->getNickname().size() ? "	[" + _clients[fd]->getNickname() + "] left the server" : "	[" + patch::to_string(fd) + "] left the server";
	std::cout << display << std::endl;
	close(fd);
	delete _clients[fd];
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

Client *Server::findClientByNicknamme(std::string nickname)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return it->second;
	}
	return NULL;
}

bool Server::correctPassword(std::string password)
{
	return (password == _password);
}

void Server::rplWelcome(Client *client)
{
	std::string reply;
	std::string reply2;
	client->setStatus(CONNECTED);
	reply += ":" + _name + " 001 " + client->getNickname() + " Welcome to the Internet Relay Network " + client->getNickname() + "\r\n";
	reply2 +=  ":" + _name + " 376 " + client->getNickname() + " End of /MOTD command\r\n";
	send(client->getPoll().fd, reply.c_str(), reply.size(), 0);
	send(client->getPoll().fd, reply2.c_str(), reply2.size(), 0);
}

void Server::changeClientClass(Client *oldClient, Client *newClient)
{
	t_pollfd newPoll;
	newPoll = oldClient->getPoll();
	newClient->setPoll(newPoll);
	_clients[oldClient->getPoll().fd] = newClient; 
}

void Server::displayServer()
{
	std::string display;
	std::string state;
	Client *client;
	Channel *channel;

    // std::cout << std::string( 100, '\n' );
	if (_clients.size() != 0)
	{
		display =  "USERS : \n";
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->second->getStatus() == CONNECTED || it->second->getStatus() == REFUSED)
			{
				client = it->second;
				if (client->getStatus() == CONNECTED)
					display += "	[" + client->getNickname() + "] " + " mode +" + client->getMode() + " CONNECTED \n";
				else if (client->getStatus() == REFUSED)
					display += "	[" + patch::to_string(client->getPoll().fd) + "] CONNECTION REFUSED \n";
				std::cout << display << std::endl;
			}
		}
		
	}
	if (_channel.size() != 0)
	{
		display =  "CHANNELS : \n";
		for (std::map<std::string, Channel *>::iterator it = _channel.begin(); it != _channel.end(); it++)
		{
			channel = it->second;
			display += "	[" + channel->getName() + "] " + patch::to_string(channel->getClients().size()) + " user(s)\n";
			std::cout << display << std::endl;
		}
		
	}
}