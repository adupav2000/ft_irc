#include "command.hpp"

void Command::insertAllMess()
{
	this->_errMess[ERR_NOSUCHNICK] = "<nickname> :No such nick/channel";
	this->_errMess[ERR_NOSUCHSERVER] = "<server name> :No such server";
	this->_errMess[ERR_NOSUCHCHANNEL] = "<channel name> :No such channel";
	this->_errMess[ERR_CANNOTSENDTOCHAN] = "<channel name> :Cannot send to channel";
	this->_errMess[ERR_TOOMANYCHANNELS] = "<channel name> :You have joined too many channels";
	this->_errMess[ERR_WASNOSUCHNICK] = "<nickname> :There was no such nickname";
	this->_errMess[ERR_TOOMANYTARGETS] = "<target> :<error code> recipients. <abort message>";
	this->_errMess[ERR_NOSUCHSERVICE] = "<service name> :No such service";
	this->_errMess[ERR_NOORIGIN] = ":No origin specified";
	this->_errMess[ERR_NORECIPIENT] = ":No recipient given (<command>)";
	this->_errMess[ERR_NOTEXTTOSEND] = ":No text to send";
	this->_errMess[ERR_NOTOPLEVEL] = "<mask> :No toplevel domain specified";
	this->_errMess[ERR_WILDTOPLEVEL] = "<mask> :Wildcard in toplevel domain";
	this->_errMess[ERR_BADMASK] = "<mask> :Bad Server/host mask";
	this->_errMess[ERR_UNKNOWNCOMMAND] = "<command> :Unknown command";
	this->_errMess[ERR_NOMOTD] = ":MOTD File is missing";
	this->_errMess[ERR_NOADMININFO] = "<server> :No administrative info available";
	this->_errMess[ERR_FILEERROR] = ":File error doing <file op> on <file>";
	this->_errMess[ERR_NONICKNAMEGIVEN] = ":No nickname given";
	this->_errMess[ERR_ERRONEUSNICKNAME] = "<nick> :Erroneous nickname";
	this->_errMess[ERR_NICKNAMEINUSE] = "<nick> :Nickname is already in use";
	this->_errMess[ERR_NICKCOLLISION] = "<nick> :Nickname collision KILL from <user>@<host>";
	this->_errMess[ERR_UNAVAILRESOURCE] = "<nick/channel> :Nick/channel is temporarily unavailable";
	this->_errMess[ERR_USERNOTINCHANNEL] = "<nick> <channel> :They aren't on that channel";
	this->_errMess[ERR_NOTONCHANNEL] = "<channel> :You're not on that channel";
	this->_errMess[ERR_USERONCHANNEL] = "<user> <channel> :is already on channel";
	this->_errMess[ERR_NOLOGIN] = "<user> :User not logged in";
	this->_errMess[ERR_SUMMONDISABLED] = ":SUMMON has been disabled";
	this->_errMess[ERR_USERSDISABLED] = ":USERS has been disabled";
	this->_errMess[ERR_NOTREGISTERED] = ":You have not registered";
	this->_errMess[ERR_NEEDMOREPARAMS] = "<command> :Not enough parameters";
	this->_errMess[ERR_ALREADYREGISTRED] = ":Unauthorized command (already registered)";
	this->_errMess[ERR_NOPERMFORHOST] = ":Your host isn't among the privileged";
	this->_errMess[ERR_PASSWDMISMATCH] = ":Password incorrect";
	this->_errMess[ERR_YOUREBANNEDCREEP] = ":You are banned from this server";
	this->_errMess[ERR_KEYSET] = "<channel> :Channel key already set";
	this->_errMess[ERR_CHANNELISFULL] = "<channel> :Cannot join channel (+l)";
	this->_errMess[ERR_UNKNOWNMODE] = "<char> :is unknown mode char to me for <channel>";
	this->_errMess[ERR_INVITEONLYCHAN] = "<channel> :Cannot join channel (+i)";
	this->_errMess[ERR_BANNEDFROMCHAN] = "<channel> :Cannot join channel (+b)";
	this->_errMess[ERR_BADCHANNELKEY] = "<channel> :Cannot join channel (+k)";
	this->_errMess[ERR_BADCHANMASK] = "<channel> :Bad Channel Mask";
	this->_errMess[ERR_NOCHANMODES] = "<channel> :Channel doesn't support modes";
	this->_errMess[ERR_BANLISTFULL] = "<channel> <char> :Channel list is full";
	this->_errMess[ERR_NOPRIVILEGES] = ":Permission Denied- You're not an IRC operator";
	this->_errMess[ERR_CHANOPRIVSNEEDED] = "<channel> :You're not channel operator";
	this->_errMess[ERR_CANTKILLSERVER] = ":You can't kill a server";
	this->_errMess[ERR_RESTRICTED] = ":Your connection is restricted";
	this->_errMess[ERR_UNIQOPPRIVSNEEDED] = ":You're not the original channel operator";
	this->_errMess[ERR_NOOPERHOST] = ":No O-lines for your host";
	this->_errMess[ERR_UMODEUNKNOWNFLAG] = ":Unknown MODE flag";
	this->_errMess[ERR_USERSDONTMATCH] = ":Cannot change mode for other users";

	this->_errMess[RPL_WELCOME] = "Welcome to the Internet Relay Network\n <nick>!<user>@<host>";
	this->_errMess[RPL_YOURHOST] = "Your host is <servername>, running version <ver>";
	this->_errMess[RPL_CREATED] = "This server was created <date>";
	this->_errMess[RPL_MYINFO] = "<servername> <version> <available user modes>\n		<available channel modes>";
	this->_errMess[RPL_BOUNCE] = "Try server <server name>, port <port number>";
	this->_errMess[RPL_USERHOST] = ":*1<reply> *( \" \" <reply> )";
	this->_errMess[RPL_ISON] = ":*1<nick> *( \" \" <nick> )";
	this->_errMess[RPL_AWAY] = "<nick> :<away message>";
	this->_errMess[RPL_UNAWAY] = ":You are no longer marked as being away";
	this->_errMess[RPL_NOWAWAY] = ":You have been marked as being away";
	this->_errMess[RPL_WHOISUSER] = "<nick> <user> <host> * :<real name>";
	this->_errMess[RPL_WHOISSERVER] = "<nick> <server> :<server info>";
	this->_errMess[RPL_WHOISOPERATOR] = "<nick> :is an IRC operator";
	this->_errMess[RPL_WHOISIDLE] = "<nick> <integer> :seconds idle";
	this->_errMess[RPL_ENDOFWHOIS] = "<nick> :End of WHOIS list";
	this->_errMess[RPL_WHOISCHANNELS] = "<nick> :*( ( \" @\" / \" \" ) <channel> \" \" )";
	this->_errMess[RPL_WHOWASUSER] = "<nick> <user> <host> * :<real name>";
	this->_errMess[RPL_ENDOFWHOWAS] = "<nick> :End of WHOWAS";
	this->_errMess[RPL_LIST] = "<channel> <# visible> :<topic>";
	this->_errMess[RPL_LISTEND] = ":End of LIST";
	this->_errMess[RPL_UNIQOPIS] = "<channel> <nickname>";
	this->_errMess[RPL_CHANNELMODEIS] = "<channel> <mode> <mode params>";
	this->_errMess[RPL_NOTOPIC] = "<channel> :No topic is set";
	this->_errMess[RPL_TOPIC] = "<channel> :<topic>";
	this->_errMess[RPL_INVITING] = "<channel> <nick>";
	this->_errMess[RPL_SUMMONING] = "<user> :Summoning user to IRC";
	this->_errMess[RPL_INVITELIST] = "<channel> <invitemask>";
	this->_errMess[RPL_ENDOFINVITELIST] = "<channel> :End of channel invite list";
	this->_errMess[RPL_EXCEPTLIST] = "<channel> <exceptionmask>";
	this->_errMess[RPL_ENDOFEXCEPTLIST] = "<channel> :End of channel exception list";
	this->_errMess[RPL_VERSION] = "<version>.<debuglevel> <server> :<comments>";
	this->_errMess[RPL_WHOREPLY] = "<channel> <user> <host> <server> <nick>";
	this->_errMess[RPL_ENDOFWHO] = "<name> :End of WHO list";
	this->_errMess[RPL_NAMREPLY] = "( '=' / '*' / '@' ) <channel>";
	this->_errMess[RPL_ENDOFNAMES] = "<channel> :End of NAMES list";
	this->_errMess[RPL_LINKS] = "<mask> <server> :<hopcount> <server info>";
	this->_errMess[RPL_ENDOFLINKS] = "<mask> :End of LINKS list";
	this->_errMess[RPL_BANLIST] = "<channel> <banmask>";
	this->_errMess[RPL_ENDOFBANLIST] = "<channel> :End of channel ban list";
	this->_errMess[RPL_INFO] = ":<string>";
	this->_errMess[RPL_ENDOFINFO] = ":End of INFO list";
	this->_errMess[RPL_MOTDSTART] = ":- <server> Message of the day - ";
	this->_errMess[RPL_MOTD] = ":- <text>";
	this->_errMess[RPL_ENDOFMOTD] = ":End of MOTD command";
	this->_errMess[RPL_YOUREOPER] = ":You are now an IRC operator";
	this->_errMess[RPL_REHASHING] = "<config file> :Rehashing";
	this->_errMess[RPL_YOURESERVICE] = "You are service <servicename>";
	this->_errMess[RPL_TIME] = "<server> :<string showing server's local time>";
	this->_errMess[RPL_USERSSTART] = ":UserID   Terminal  Host";
	this->_errMess[RPL_USERS] = ":<username> <ttyline> <hostname>";
	this->_errMess[RPL_ENDOFUSERS] = ":End of users";
	this->_errMess[RPL_NOUSERS] = ":Nobody logged in";
	this->_errMess[RPL_TRACELINK] = "Link <version & debug level> <destination>";
	this->_errMess[RPL_TRACECONNECTING] = "Try. <class> <server>";
	this->_errMess[RPL_TRACEHANDSHAKE] = "H.S. <class> <server>";
	this->_errMess[RPL_TRACEUNKNOWN] = "???? <class> [<client IP address in dot form>]";
	this->_errMess[RPL_TRACEOPERATOR] = "Oper <class> <nick>";
	this->_errMess[RPL_TRACEUSER] = "User <class> <nick>";
	this->_errMess[RPL_TRACESERVER] = "Serv <class> <int>S <int>C <server>\n		<nick!user|*!*>@<host|server> V<protocol version>";
	this->_errMess[RPL_TRACESERVICE] = "Service <class> <name> <type> <active type>";
	this->_errMess[RPL_TRACENEWTYPE] = "<newtype> 0 <client name>";
	this->_errMess[RPL_TRACECLASS] = "Class <class> <count>";
	this->_errMess[RPL_TRACELOG] = "File <logfile> <debug level>";
	this->_errMess[RPL_TRACEEND] = "<server name> <version & debug level> :End of TRACE";
	this->_errMess[RPL_STATSLINKINFO] = "<linkname> <sendq> <sent messages>\n		<sent Kbytes> <received messages>\n <received Kbytes> <time open>";
	this->_errMess[RPL_STATSCOMMANDS] = "<command> <count> <byte count> <remote count>";
	this->_errMess[RPL_ENDOFSTATS] = "<stats letter> :End of STATS report";
	this->_errMess[RPL_STATSUPTIME] = ":Server Up %d days %d:%02d:%02d";
	this->_errMess[RPL_STATSOLINE] = "O <hostmask> * <name>";
	this->_errMess[RPL_UMODEIS] = "<server address> 221 <nickname> :<new_mode_string>";
	this->_errMess[RPL_SERVLIST] = "<name> <server> <mask> <type> <hopcount> <info>";
	this->_errMess[RPL_SERVLISTEND] = "<mask> <type> :End of service listing";
	this->_errMess[RPL_LUSERCLIENT] = ":There are <integer> users and <integer>";
	this->_errMess[RPL_LUSEROP] = "<integer> :operator(s) online";
	this->_errMess[RPL_LUSERUNKNOWN] = "<integer> :unknown connection(s)";
	this->_errMess[RPL_LUSERCHANNELS] = "<integer> :channels formed";
	this->_errMess[RPL_LUSERME] = ":I have <integer> clients and <integer>";
	this->_errMess[RPL_ADMINME] = "<server> :Administrative info";
	this->_errMess[RPL_ADMINLOC1] = ":<admin info>";
	this->_errMess[RPL_ADMINLOC2] = ":<admin info>";
	this->_errMess[RPL_ADMINEMAIL] = ":<admin info>";
	this->_errMess[RPL_TRYAGAIN] = "<command> :Please wait a while and try again.";
	this->_errMess[SEND_CONFIRMNEWNICK] = "Your nickname is now <nickname>";
	
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
}

Command::Command(Command const & rhs)
{
	this->_errMess = rhs._errMess;
	this->_prefix = rhs._prefix;
	this->_command = rhs._command;
	this->_parameters = rhs._parameters;
	this->_message = rhs._message;
	this->_recv = rhs._recv;
	this->_client = rhs._client;
	this->_serverRef = rhs._serverRef;
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
	if (this->_errMess.find(num) != this->_errMess.end())
		return ("");
	std::string ret = this->_errMess.at(num);
	ret = findAndReplace("<nickname>", ret, _client->getNickname());
	if (this->getPrefix() == "NICK" && this->getParameters().size() > 0)
		ret = findAndReplace("<nick", ret, this->getParameters()[0]);
	ret = findAndReplace("<server name>", ret, _serverRef->getName());
	ret = findAndReplace("<command>", ret, this->getPrefix());
	ret = findAndReplace("<channel>", ret, this->getPrefix());
	ret = findAndReplace("<topic>", ret, this->getPrefix());
	/* KWAME to be set up wth functions */
	/*
	EXAMPLE
	std::string str = "<channel name>";
	ret.replace(ret.find(str), str.length(), _channel->getName());
	str = "<target>";
	str = "<error code>";
	str = "<mask>";
	str = "<service name>";
	ret.replace(ret.find(str), str.length(), _client->getNickname());
	*/
	return (ret);
}