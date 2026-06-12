//hamza

#include "Server.hpp"

static bool isValidNick(const std::string &nick)
{
	if (nick.empty())
		return false;
	for (size_t i = 0; i < nick.size(); i++)
	{
		unsigned char ch = static_cast<unsigned char>(nick[i]);
		if (!std::isprint(ch) || std::isspace(ch) || ch == ':' || ch == ',' || ch == '!' || ch == '@')
			return false;
	}
	if (nick[0] == '#')
		return false;
	return true;
}

void Server::cmdPass(Client *c, const std::vector<std::string> &args)
{
	if (c->hasPasswd() || c->isRegistered())
		return sendNumeric(c, ERR_ALREADYREGISTERED, ":Already registered");
	if (args.empty())
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "PASS :Not enough parameters");
	if (args[0] != _password)
		return sendNumeric(c, ERR_PASSWDMISMATCH, ":Password incorrect");
	c->setPasswd(true);
	tryRegister(c);
}

void Server::cmdNick(Client *c, const std::vector<std::string> &args)
{
	if (args.empty())
		return sendNumeric(c, ERR_NONICKNAMEGIVEN, ":No nickname given");
	const std::string &newNick = args[0];
	if (!isValidNick(newNick))
		return sendNumeric(c, ERR_ERRONEUSNICKNAME, newNick + " :Erroneous nickname");
	Client *existing = findByNick(newNick);
	if (existing && existing != c)
		return sendNumeric(c, ERR_NICKNAMEINUSE, newNick + " :Nickname is already in use");

	std::string oldPrefix = c->prefix();
	std::string oldNick = c->getNick();
	c->setNick(newNick);
	tryRegister(c);

	if (!c->hasWelcomed() || oldNick.empty() || oldNick == newNick)
		return;

	std::string msg = ":" + oldPrefix + " NICK :" + newNick + "\r\n";
	sendReply(c, msg);
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		Channel *ch = it->second;
		if (ch->isMember(c))
			ch->broadcast(*this, msg, c);
	}
}

void Server::cmdUser(Client *c, const std::vector<std::string> &args)
{
	if (c->hasUser() || c->isRegistered())
		return sendNumeric(c, ERR_ALREADYREGISTERED, ":Already registered");
	if (args.size() < 4)
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
	if (!c->hasPasswd())
	{
		sendNumeric(c, ERR_PASSWDMISMATCH, ":Password incorrect");
		c->setPasswd(false);
		return;
	}
	c->setUser(args[0]);
	c->setReal(args[3]);
	tryRegister(c);
}

void Server::cmdQuit(Client *c, const std::vector<std::string> &args)
{
	(void)args;
	int fd = c->getFd();
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == fd)
		{
			disconnectClient(i);
			return;
		}
	}
}
void Server::cmdPing(Client *c, const std::vector<std::string> &args)
{
	if (args.empty())
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "PING :Not enough parameters");
	sendReply(c, ":ircserv PONG ircserv :" + args[0] + "\r\n");
}
