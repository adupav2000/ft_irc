#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <string>
#include <iostream>
class command
{
public:
    command();
    command(command const & rhs);
    ~command();
    command &operator=(command const & rhs);

private:

};

#endif