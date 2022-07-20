#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include "../globals.hpp"

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

private:
    std::string _prefix;
    std::string _command;
    std::vector<std::string> _parameters;
    std::string _message;
    std::string _recv;
};

#endif