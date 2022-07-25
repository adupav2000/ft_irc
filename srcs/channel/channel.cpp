#include "channel.hpp"

Channel::Channel()
{
    return ;
}

Channel::Channel(std::string name, Server *server, Client *client) : _name(name), _topic(""), _mode("="), _server(server), _operator(client)
{
    return ;
}


Channel::Channel(Channel const & rhs)
{
    (void)rhs;
    return ;
}

Channel::~Channel()
{
    return ;
}

// Channel &Channel::operator=(Channel const & rhs)
// {
//     return 
// }

/* GETTERS */
std::string Channel::getName()
{
    return _name;
}

std::string Channel::getTopic()
{
    return _topic;
}

std::string Channel::getMode()
{
    return _mode;
}

std::string Channel::getKey()
{
    return _key;
}

unsigned int Channel::getNbClients()
{
    return _nbClients;
}

std::map<int, Client *> Channel::getClients()
{
    return _clients;
}

std::string Channel::getClientsName(Channel *channel)
{
    std::string ret;
    std::map<int, Client *> clients = channel->getClients();
    Client *cli;
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        cli = (*it).second;
        if (cli != clients.begin()->second)
            ret += " ";
        if (cli == _operator)
            ret += "@";
        ret += cli->getNickname();
    }
    return ret;  
}

std::string Channel::getMaxClients()
{
    return _maxClients;
}

/* SETTERS */

void Channel::setKey(std::string key)
{
    _key = key;
}


void Channel::addToChannel(Client *client)
{
    _clients.insert(std::pair<int, Client * >(client->getPoll().fd, client));
    _nbClients++;
}