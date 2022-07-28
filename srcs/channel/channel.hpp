#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include <iostream>

#include "../server/server.hpp"
#include "../client/client.hpp"
#include "../command/command.hpp"
#include "../globals.hpp"

class Server;
class Client;
class Command;

class Channel
{
    public:
        Channel();
        Channel(std::string name, Server *server, Client *client);
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
		std::string getMaxClients();

        /* SETTERS */
        void setKey(std::string key);
		void setTopic(std::string topic);


        /* CHANNEL OPERATION */
        // int PART(Command *arguments);
        void MODE();
        void TOPIC();
        void NAMES();
        void LIST();
        void INVITE();
        void QUIT();

        /* Channel function */
        void addToChannel(Client *client);
        std::string getClientsName(Channel *channel);
        void removeFromChannel(Client *client);
        bool clientOnChannel(std::string name);
        void addInvited(Client *client);



    private:
		std::string _name;
		std::string _topic;
		std::string _mode;
		std::string _key;
        unsigned int _nbClients;
		std::map<int, Client *> _clients;
		Server *_server;
        Client * _operator;
		std::string _maxClients;
        std::vector<Client *> _invited;
};

// int JOIN(Command *arguments);

#endif
