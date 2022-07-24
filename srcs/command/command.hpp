#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include "../server/server.hpp"
#include "../client/client.hpp"
#include "../globals.hpp"

class Server;
class Client;

#define MAX_PARAMS 15

class Command
{
public:
    Command();
    Command(std::string message);
    Command(Command const & rhs);
    ~Command();
    Command &operator=(Command const & rhs);

    // GETTERS
    std::string getPrefix();
    std::string getCommand();
    std::vector<std::string> getParameters();
	std::string getErrorString(int num);

private:
	std::map<int, std::string> _errMess;
    std::string _prefix;
    std::string _command;
    std::vector<std::string> _parameters;
    std::string _message;
    std::string _recv;

	Client		*_client;
	Server		*_serverRef;
	Channel		*_channel;
};

#endif