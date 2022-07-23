#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <iostream>

#include "../server/server.hpp"
#include "../client/client.hpp"
#include "../globals.hpp"
#include "../command/command.hpp"

class Server;
class Client;

class Channel
{
    public:
        Channel();
        Channel(Channel const & rhs);
        ~Channel();
        Channel &operator=(Channel const & rhs);

        /* GETTERS */
        std::string getName();
		std::string getTopic();
		std::string getMode();
		std::string getKey();
        unsigned int getNbClients();
		std::map<int, Client *> getClients();
		std::string getMaxUsers();


    private:
		std::string _name;
		std::string _topic;
		std::string _mode;
		std::string _key;
        unsigned int _nbClients;
		std::map<int, Client *> _clients;
		std::string _maxUsers;
};

#endif
