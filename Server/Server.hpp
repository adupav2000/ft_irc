#ifndef Server_HPP
# define Server_HPP

#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <poll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <string.h>    // for strncmp
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
class Server
{
    public:
        Server();
        // Server(Server const & rhs);
        ~Server();
        // Server &operator=(Server const & rhs);

        void init();
        void launch();
        /* Handle incomming strings */
        // in charge of calling the right function
        // int handle_string(std::string input);

        /* client operation */
        // int new_client(); // sends a PL_WELCOME and adds a client in map
        // int new_service(); // sends a RPL_YOURESERVICE and adds a service in maps
        // int register_password();

        

    private:
        std::string _name;
        struct pollfd _fds;

        //Client map<int, std::string> client_list;
        //Service map<int, std::string> service_list;
};

#endif