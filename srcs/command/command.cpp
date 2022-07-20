#include "command.hpp"

Command::Command()
{
	return;
}

Command::Command(std::string message) : _recv(message)
{
	std::vector<std::string> tmp;

	tmp = split(message, ":");
	_parameters = split(tmp[0], "0");
	// std::cout <<  _parameters << std::endl;
	_prefix = *(_parameters.begin());
	std::cout << "prefix : " << _prefix << std::endl;
	_parameters.erase(_parameters.begin());
	if (tmp.size() > 1)
		message = tmp[1];
	for (size_t i = 0; i < _prefix.length(); i++)
	{
		_prefix[i] = std::toupper(_prefix[i]);
	}
	std::cout << "prefix : " << _prefix << std::endl;
	return ;
}

Command::~Command()
{
	return;
}

// Command::Command(Command const & rhs)
// {

// }

// Command &Command::operator=(Command const & rhs)
// {

// }

// GETTERS
std::string Command::getPrefix()
{
	return _prefix;
}

std::string Command::getCommand()
{
	return _command;
}

std::vector<std::string> Command::getParameters()
{
	return _parameters;
}
