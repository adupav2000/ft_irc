#include "command.hpp"

void Command::insertAllMess()
{
	this->_errMess[ERR_NOSUCHNICK] = "<nick> :No such nick/channel";
	this->_errMess[ERR_NOSUCHSERVER] = "<server name> :No such server";
	this->_errMess[ERR_NOSUCHCHANNEL] = "<channel name> :No such channel";
	this->_errMess[ERR_CANNOTSENDTOCHAN] = "<channel name> :Cannot send to channel";
	this->_errMess[ERR_TOOMANYCHANNELS] = "<channel name> :You have joined too many channels";
	this->_errMess[ERR_WASNOSUCHNICK] = "<nick> :There was no such nickname";
	this->_errMess[ERR_NOORIGIN] = ":No origin specified";
	this->_errMess[ERR_NOTEXTTOSEND] = ":No text to send";
	this->_errMess[ERR_UNKNOWNCOMMAND] = "<command> :Unknown command";
	this->_errMess[ERR_NOMOTD] = ":MOTD File is missing";
	this->_errMess[ERR_NONICKNAMEGIVEN] = ":No nickname given";
	this->_errMess[ERR_ERRONEUSNICKNAME] = "<nick> :Erroneous nickname";
	this->_errMess[ERR_NICKNAMEINUSE] = "<nickname> <nick> :Nickname is already in use";
	this->_errMess[ERR_USERNOTINCHANNEL] = "<nickname> <nick> <channel> :They aren't on that channel";
	this->_errMess[ERR_NOTONCHANNEL] = "<channel> :You're not on that channel";
	this->_errMess[ERR_USERONCHANNEL] = "<user> <channel> :is already on channel";
	this->_errMess[ERR_NOTREGISTERED] = ":You have not registered";
	this->_errMess[ERR_NEEDMOREPARAMS] = "<command> :Not enough parameters";
	this->_errMess[ERR_ALREADYREGISTRED] = ":Unauthorized command (already registered)";
	this->_errMess[ERR_PASSWDMISMATCH] = ":Password incorrect";
	this->_errMess[ERR_YOUREBANNEDCREEP] = ":You are banned from this server";
	this->_errMess[ERR_CHANNELISFULL] = "<channel> :Cannot join channel (+l)";
	this->_errMess[ERR_UNKNOWNMODE] = "<char> :is unknown mode char to me for <channel>";
	this->_errMess[ERR_INVITEONLYCHAN] = "<channel> :Cannot join channel (+i)";
	this->_errMess[ERR_BANNEDFROMCHAN] = "<channel> :Cannot join channel (+b)";
	this->_errMess[ERR_BADCHANNELKEY] = "<channel> :Cannot join channel (+k)";
	this->_errMess[ERR_BADCHANMASK] = "<channel> :Bad Channel Mask";
	this->_errMess[ERR_NOPRIVILEGES] = ":Permission Denied- You're not an IRC operator";
	this->_errMess[ERR_CHANOPRIVSNEEDED] = "<channel> :You're not channel operator";
	this->_errMess[ERR_CANTKILLSERVER] = ":You can't kill a server";
	this->_errMess[ERR_RESTRICTED] = ":Your connection is restricted";
	this->_errMess[ERR_NOOPERHOST] = ":No O-lines for your host";
	this->_errMess[ERR_UMODEUNKNOWNFLAG] = ":Unknown MODE flag";
	this->_errMess[ERR_USERSDONTMATCH] = ":Cannot change mode for other users";


	this->_errMess[RPL_ENDOFWHOIS] = "<nick> :End of WHOIS list";

	 this->_errMess[RPL_VERSION] = "<version>.0 <server> :<comments>";

	this->_errMess[RPL_INFO] = ":<nickname>!<username>@<hostname> 42 Project made by adu-pavi and kamanfo\r\n";
	this->_errMess[RPL_ENDOFINFO] = ":End of INFO list";
	 this->_errMess[RPL_MOTDSTART] = ":- <server> Message of the day - ";

	 this->_errMess[RPL_ENDOFMOTD] = ":End of MOTD command";
	this->_errMess[RPL_YOUREOPER] = ":You are now an IRC operator";

	this->_errMess[RPL_YOURESERVICE] = "You are service <servicename>";

	this->_errMess[SEND_CONFIRMNEWNICK] = "Your nickname is now <nickname>";

	this->_errMess[SEND_CONFIRMDISCONNECT] = "Your connection was suspended";
}

Command::Command()
{
	this->insertAllMess();
	return;
}

Command::Command(std::string message, Server *server, Client *client) : _message(""), _recv(message), _serverRef(server), _client(client)
{
	this->insertAllMess();
	std::vector<std::string> tmp;
	if (message.find(":") != std::string::npos)
		_message = ":";
	tmp = split(message, ":");
	_parameters = split(tmp[0], " ");
	_prefix = _parameters[0];
	_parameters.erase(_parameters.begin());
	if (tmp.size() > 1)
		_message = tmp[1];
	for (size_t i = 0; i < _prefix.length(); i++)
	{
		_prefix[i] = std::toupper(_prefix[i]);
	}
	_channel = NULL;
}

Command::Command(Command const &rhs)
{
	this->_errMess = rhs._errMess;
	this->_prefix = rhs._prefix;
	this->_command = rhs._command;
	this->_parameters = rhs._parameters;
	this->_message = rhs._message;
	this->_recv = rhs._recv;
	this->_client = rhs._client;
	this->_serverRef = rhs._serverRef;
	this->_channel = rhs._channel;
}

Command::~Command()
{
	return;
}

// GETTERS
std::string Command::getPrefix() const
{
	return _prefix;
}

std::string Command::getCommand() const
{
	return _command;
}

std::string Command::getMessage() const
{
	return _message;
}

Client *Command::getClient() const
{
	return _client;
}

Server *Command::getServer() const
{
	return _serverRef;
}

Channel *Command::setChannel(Channel *chan)
{
	return (_channel = chan);
}


std::vector<std::string> Command::getParameters() const
{
	return _parameters;
}

std::string findAndReplace(std::string toBeFound, std::string toBeSearchedThrought, std::string toReplaceWith)
{
	std::string ret = toBeSearchedThrought;
	if (ret.find(toBeFound) != std::string::npos && toReplaceWith != "")
	{
		ret.replace(ret.find(toBeFound), toBeFound.length(), toReplaceWith);
	}
	return (ret);
}

/**
 * @brief Get the String Command (ths whole command as it was sent)
 *
 * @return std::string
 */
std::string Command::getStringCommand() const
{
	return (this->_recv);
}

std::string Command::getErrorString(int num) const
{
	if (this->_errMess.find(num) == this->_errMess.end())
		return ("");
	std::string ret = this->_errMess.at(num);
	if (_client->getNickname().length() == 0)
		ret = findAndReplace("<nickname>", ret, _client->getUsername());
	else 
		ret = findAndReplace("<nickname>", ret, _client->getNickname());
	if (this->getParameters().size() > 0)
		ret = findAndReplace("<nick>", ret, this->getParameters()[0]);
	if (this->getPrefix() == "KICK" && this->getParameters().size() >= 2)
		ret = findAndReplace("<nick>", ret, this->getParameters()[1]);
	ret = findAndReplace("<server name>", ret, _serverRef->getName());
	ret = findAndReplace("<command>", ret, this->getPrefix());
	if (this->_channel != NULL)
		ret = findAndReplace("<channel>", ret, this->_channel->getName());
	ret = findAndReplace("<channel name>", ret, this->getPrefix());
	ret = findAndReplace("<topic>", ret, this->getPrefix());

	ret = findAndReplace("<version>", ret, this->_serverRef->getVersion());
    ret = findAndReplace("<server>", ret, this->_serverRef->getName());
    ret = findAndReplace("<comments>", ret, "Fully functionnal !");
	if (ret.size() > 2 &&
		((*(ret.end() - 2) != '\n' || *(ret.end() - 2) != '\r')
		&& (*(ret.end() - 1) != '\n' || *(ret.end() - 1) != '\r')))
	{
		ret.push_back('\r');
		ret.push_back('\n');
	}
	return (ret);
}