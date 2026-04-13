//hamza

#include "Server.hpp"

static bool isValidChannelName(const std::string &name)
{
	if (name.empty() || name[0] != '#')
		return false;
	for (size_t i = 0; i < name.size(); i++)
	{
		unsigned char ch = static_cast<unsigned char>(name[i]);
		if (!std::isprint(ch) || std::isspace(ch) || ch == ',' || ch == ':')
			return false;
	}
	return true;
}

static void sendNamesFor(Server &server, Client *c, Channel *ch)
{
	std::vector<Client*> members = ch->getMembers();
	std::string names;
	for (size_t i = 0; i < members.size(); i++)
	{
		if (i != 0)
			names += " ";
		if (ch->isOperator(members[i]))
			names += "@";
		names += members[i]->getNick();
	}
	server.sendNumeric(c, RPL_NAMREPLY, "= " + ch->getName() + " :" + names);
	server.sendNumeric(c, RPL_ENDOFNAMES, ch->getName() + " :End of /NAMES list");
}

void Server::cmdJoin(Client *c, const std::vector<std::string> &args)
{
	if (args.empty())
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
	if (args[0] == "0")
		return;

	std::string channelName = args[0];
	if (!isValidChannelName(channelName))
		return sendNumeric(c, ERR_NOSUCHCHANNEL, channelName + " :No such channel");

	Channel *ch = getChannel(channelName);
	if (!ch)
		ch = createChannel(channelName);

	if (ch->isMember(c))
		return;

	if (ch->memberCount() > 0 && ch->isInviteOnly() && !ch->isInvited(c))
		return sendNumeric(c, ERR_INVITEONLYCHAN, channelName + " :Cannot join channel (+i)");

	std::string providedKey;
	if (args.size() > 1)
		providedKey = args[1];
	if (ch->hasKey() && providedKey != ch->getKey())
		return sendNumeric(c, ERR_BADCHANNELKEY, channelName + " :Cannot join channel (+k)");
	if (ch->hasLimit() && ch->memberCount() >= ch->getLimit())
		return sendNumeric(c, ERR_CHANNELISFULL, channelName + " :Cannot join channel (+l)");

	bool first = (ch->memberCount() == 0);
	ch->addMember(c);
	if (first)
		ch->addOperator(c);

	std::string joinMsg = ":" + c->prefix() + " JOIN :" + channelName + "\r\n";
	ch->broadcast(*this, joinMsg, NULL);

	if (ch->getTopic().empty())
		sendNumeric(c, RPL_NOTOPIC, channelName + " :No topic is set");
	else
		sendNumeric(c, RPL_TOPIC, channelName + " :" + ch->getTopic());

	sendNamesFor(*this, c, ch);
}

void Server::cmdNames(Client *c, const std::vector<std::string> &args)
{
	if (args.empty())
	{
		for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
			sendNamesFor(*this, c, it->second);
		return;
	}

	std::string channelName = args[0];
	Channel *ch = getChannel(channelName);
	if (!ch)
		return sendNumeric(c, ERR_NOSUCHCHANNEL, channelName + " :No such channel");
	sendNamesFor(*this, c, ch);
}
