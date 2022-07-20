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
	(void)argc;
	(void)argv;

	server test;
	test.init();
	return 0;
}