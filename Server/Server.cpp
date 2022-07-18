#include "./Server.hpp"

Server::Server() : _name(), _fds()
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
	if (fcntl(socketfd, F_SETFL, O_NONBLOCK) == -1)
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
	int								client_sock;
	struct sockaddr_storage 		client_addr;
	socklen_t						addr_size;
	std::vector<struct pollfd>		pollfds;
	struct pollfd					client;
	std::vector<struct pollfd>		users;
	char							buffer[1025];

	for (;;)
	{
		pollfds.push_back(_fds);
		if (poll(&pollfds[0], pollfds.size() + 1, -1) == -1)
			strerror(errno);
		for (std::vector<struct pollfd>::iterator it = users.begin(); it != users.end(); it++)
		{
			pollfds.push_back(*it);
		}
		std::vector<pollfd>::iterator beg = pollfds.begin();
		std::vector<pollfd>::iterator end = pollfds.end();
		for (; beg != end; beg++)
		{
			if (beg->revents != 0)
			{
				std::cout << "fd : revents " << beg->fd << " " << beg->revents << std::endl;

				if (beg->fd == _fds.fd)
				{
					std::cout << "server :" << std::endl;
					addr_size = sizeof(client_addr);
					client_sock = accept(_fds.fd, (struct sockaddr *)&client_addr, &addr_size);
					if (client_sock == -1)
						strerror(errno);
					if (fcntl(client_sock, F_SETFL,  O_NONBLOCK) == -1)
						strerror(errno);	
					client.fd = client_sock;
					client.events = POLLIN | POLLHUP;
					users.push_back(client);
					pollfds.push_back(client);
					std::cout << "nb client : " << pollfds.size() << std::endl;
				}
				else {
					std::cout << "client" << std::endl;
					for (std::vector<pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); it ++)
					{
						if (it->fd == beg->fd)
							client = *it;
					}
					std::cout << client.fd << std::endl;
					std::cout << "Client: ";
					recv(client_sock, buffer, 1024, 0);
					std::cout << buffer << " ";
					std::cout << "" << std::flush;
				}
			}
		}
		pollfds.clear();
	}
}