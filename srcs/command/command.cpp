#include "command.hpp"

Command::Command()
{
	return;
}

Command::Command(std::string message) : _recv(message)
{
	std::vector<std::string> tmp;
	// std::cout << "message " << message << std::endl;
	tmp = split(message, ":");
	_parameters = split(tmp[0], " ");
	_prefix = _parameters[0];
	_parameters.erase(_parameters.begin());
	// for (std::vector<std::string>::iterator it = _parameters.begin(); it != _parameters.end(); it++)
	// {
	// 	std::cout << "parameters : " << *it << std::endl;
	// }
	if (tmp.size() > 1)
	{
		_message = tmp[1];
		// std::cout << "message : " << _message << std::endl;
	}
	for (size_t i = 0; i < _prefix.length(); i++)
	{
		_prefix[i] = std::toupper(_prefix[i]);
	}
	// std::cout << "prefix : " << _prefix << std::endl;
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
