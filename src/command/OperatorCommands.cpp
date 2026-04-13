//abderahman

#include "Server.hpp"

void Server::cmdInvite(Client *c, const std::vector<std::string> &args)
{
	if (args.size() < 2)
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters");

	Client *target = findByNick(args[0]);
	if (!target)
		return sendNumeric(c, ERR_NOSUCHNICK, args[0] + " :No such nick");

	Channel *ch = getChannel(args[1]);
	if (!ch)
		return sendNumeric(c, ERR_NOSUCHCHANNEL, args[1] + " :No such channel");

	if (!ch->isMember(c))
		return sendNumeric(c, ERR_NOTONCHANNEL, ch->getName() + " :You're not on that channel");
	if (ch->isInviteOnly() && !ch->isOperator(c))
		return sendNumeric(c, ERR_CHANOPRIVSNEEDED, ch->getName() + " :You're not channel operator");
	if (ch->isMember(target))
		return sendNumeric(c, ERR_USERONCHANNEL, target->getNick() + " " + ch->getName() + " :is already on channel");

	ch->invite(target);
	sendNumeric(c, RPL_INVITING, target->getNick() + " " + ch->getName());
	sendReply(target, ":" + c->prefix() + " INVITE " + target->getNick() + " :" + ch->getName() + "\r\n");
}

void Server::cmdTopic(Client *c, const std::vector<std::string> &args)
{
	if (args.empty())
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters");

	Channel *ch = getChannel(args[0]);
	if (!ch)
		return sendNumeric(c, ERR_NOSUCHCHANNEL, args[0] + " :No such channel");

	if (args.size() == 1)
	{
		if (ch->getTopic().empty())
			return sendNumeric(c, RPL_NOTOPIC, ch->getName() + " :No topic is set");
		return sendNumeric(c, RPL_TOPIC, ch->getName() + " :" + ch->getTopic());
	}

	if (!ch->isMember(c))
		return sendNumeric(c, ERR_NOTONCHANNEL, ch->getName() + " :You're not on that channel");
	if (ch->isTopicRestricted() && !ch->isOperator(c))
		return sendNumeric(c, ERR_CHANOPRIVSNEEDED, ch->getName() + " :You're not channel operator");

	ch->setTopic(args[1]);
	ch->broadcast(*this, ":" + c->prefix() + " TOPIC " + ch->getName() + " :" + args[1] + "\r\n", NULL);
}

void Server::cmdKick(Client *c, const std::vector<std::string> &args)
{
	if (args.size() < 2)
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "KICK :Not enough parameters");

	Channel *ch = getChannel(args[0]);
	if (!ch)
		return sendNumeric(c, ERR_NOSUCHCHANNEL, args[0] + " :No such channel");

	if (!ch->isMember(c))
		return sendNumeric(c, ERR_NOTONCHANNEL, ch->getName() + " :You're not on that channel");
	if (!ch->isOperator(c))
		return sendNumeric(c, ERR_CHANOPRIVSNEEDED, ch->getName() + " :You're not channel operator");

	Client *target = findByNick(args[1]);
	if (!target)
		return sendNumeric(c, ERR_NOSUCHNICK, args[1] + " :No such nick");
	if (!ch->isMember(target))
		return sendNumeric(c, ERR_USERNOTINCHANNEL, target->getNick() + " " + ch->getName() + " :They aren't on that channel");

	std::string reason = target->getNick();
	if (args.size() > 2 && !args[2].empty())
		reason = args[2];

	std::string msg = ":" + c->prefix() + " KICK " + ch->getName() + " " + target->getNick() + " :" + reason + "\r\n";
	ch->broadcast(*this, msg, NULL);
	ch->removeMember(target);
	if (ch->memberCount() == 0)
		removeChannel(ch->getName());
}
