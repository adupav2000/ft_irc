#include "channel.hpp"

Channel::Channel()
{
    return ;
}

Channel::Channel(Channel const & rhs)
{
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

std::string Channel::getMaxClients()
{
    return _maxClients;
}