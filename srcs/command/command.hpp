#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include "../server/server.hpp"
#include "../client/client.hpp"
#include "../channel/channel.hpp"
#include "../globals.hpp"

class Server;
class Client;
class Channel;

#define MAX_PARAMS 15

class Command
{
public:
    Command();
    Command(std::string message, Server *server, Client *client);
    Command(Command const & rhs);
    ~Command();
    Command &operator=(Command const & rhs);

    /* Getters */
    std::string getPrefix() const;
    std::string getCommand() const;
	std::string getMessage() const;
    Client *getClient() const;
    Server *getServer() const;
    std::vector<std::string> getParameters() const;
	std::string getErrorString(int num) const;


private:
	void insertAllMess();
	std::map<int, std::string> _errMess;
    std::string _prefix;
    std::string _command;
    std::vector<std::string> _parameters;
    std::string _message;
    std::string _recv;

	Server	*_serverRef;
	Client *_client;
	Channel *_channel;
};

#endif
