// Example code: A simple server side code, which echos back the received message.
// Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>	   //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include "server/server.hpp"

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "le programme doit etre appelé comme ceci : ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	Server serv = Server(argv[1], argv[2]);
	serv.init();
	serv.launch();
	return 0;
}