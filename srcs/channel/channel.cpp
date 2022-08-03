#include "channel.hpp"

Channel::Channel()
{
    return ;
}

Channel::Channel(std::string name, Server *server, Client *client) : _name(name), _topic(""), _mode("="), _server(server), _operator(client), _channelModes("OovaimnqpsrtklbeI")
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

unsigned int Channel::getMaxClients()
{
    return _maxClients;
}

std::map<int, std::string> Channel::getUserMode()
{
	return _userMode;
}

std::string		Channel::getChannelModes()
{
	return _channelModes;
}

/* SETTERS */

void Channel::setKey(std::string key, char signe)
{
	if (signe == '+')
    	_key = key;
	if (signe == '-')
		_key.erase();
}

void Channel::setTopic(std::string topic)
{
    std::cout << "topic : " << topic << std::endl; 
    std::cout << "_topic : " << _topic << std::endl; 
    _topic = topic;
}

void Channel::setMaxClients(unsigned int limit)
{
	_maxClients = limit;
}

void Channel::setMode(char mode, char signe)
{
	if (signe == '-')
		_mode.erase(_mode.find(mode));
	else if (signe == '+')
		_mode += mode;
}

void Channel::addToChannel(Client *client)
{
    _clients.insert(std::pair<int, Client * >(client->getPoll().fd, client));
    _nbClients++;
}

void Channel::removeFromChannel(Client *client)
{
    //delete client;
    _clients.erase(client->getPoll().fd);
}

bool Channel::clientOnChannel(std::string name)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
		if (it->second->getNickname() == name)
			return true;
    }
	return false;
}

Client *Channel::getClientOnChannel(std::string name)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
		if (it->second->getNickname() == name)
			return it->second;
    }
	return NULL;
}

void Channel::addInvited(Client *client)
{
	_invited.push_back(client);
}

void Channel::changeUserMode(int index, char mode, char signe)
{
	if (signe == '-')
		_userMode[index].erase(_userMode[index].find(mode, 1));
	else if (signe == '+')
		_userMode[index].push_back(mode);
}

bool Channel::isInvited(std::string name)
{
	for (std::vector<Client *>::iterator it = _invited.begin(); it != _invited.end(); it++)
	{
		std::cout << "invited " << (*it)->getNickname() << std::endl;
		if ((*it)->getNickname() == name)
			return true;
	}
	return false;
	
}

