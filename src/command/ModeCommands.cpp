//abderahman

#include "Server.hpp"

static std::string toString(size_t n)
{
	std::ostringstream oss;
	oss << n;
	return oss.str();
}

void Server::cmdMode(Client *c, const std::vector<std::string> &args)
{
	if (args.empty())
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");

	Channel *ch = getChannel(args[0]);
	if (!ch)
		return sendNumeric(c, ERR_NOSUCHCHANNEL, args[0] + " :No such channel");

	if (args.size() == 1)
	{
		std::string modes = "+";
		std::string params;
		if (ch->isInviteOnly())
			modes += "i";
		if (ch->isTopicRestricted())
			modes += "t";
		if (ch->hasKey())
		{
			modes += "k";
			params += " " + ch->getKey();
		}
		if (ch->hasLimit())
		{
			modes += "l";
			params += " " + toString(ch->getLimit());
		}
		return sendNumeric(c, RPL_CHANNELMODEIS, ch->getName() + " " + modes + params);
	}

	if (!ch->isMember(c))
		return sendNumeric(c, ERR_NOTONCHANNEL, ch->getName() + " :You're not on that channel");
	if (!ch->isOperator(c))
		return sendNumeric(c, ERR_CHANOPRIVSNEEDED, ch->getName() + " :You're not channel operator");

	std::string modeStr = args[1];
	bool adding = true;
	size_t argIndex = 2;
	std::string params;

	for (size_t i = 0; i < modeStr.size(); i++)
	{
		char m = modeStr[i];
		if (m == '+')
		{
			adding = true;
			continue;
		}
		if (m == '-')
		{
			adding = false;
			continue;
		}
		if (m == 'i')
		{
			ch->setInviteOnly(adding);
			continue;
		}
		if (m == 't')
		{
			ch->setTopicRestricted(adding);
			continue;
		}
		if (m == 'k')
		{
			if (adding)
			{
				if (argIndex >= args.size())
					return sendNumeric(c, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
				ch->setKey(args[argIndex]);
				params += " " + args[argIndex];
				argIndex++;
			}
			else
			{
				ch->clearKey();
				if (argIndex < args.size())
				{
					params += " " + args[argIndex];
					argIndex++;
				}
			}
			continue;
		}
		if (m == 'l')
		{
			if (adding)
			{
				if (argIndex >= args.size())
					return sendNumeric(c, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
				int limit = std::atoi(args[argIndex].c_str());
				if (limit <= 0)
					return sendNumeric(c, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
				ch->setLimit(static_cast<size_t>(limit));
				params += " " + args[argIndex];
				argIndex++;
			}
			else
			{
				ch->clearLimit();
			}
			continue;
		}
		if (m == 'o')
		{
			if (argIndex >= args.size())
				return sendNumeric(c, ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
			std::string nick = args[argIndex];
			params += " " + nick;
			argIndex++;
			Client *target = findByNick(nick);
			if (!target)
				return sendNumeric(c, ERR_NOSUCHNICK, nick + " :No such nick");
			if (!ch->isMember(target))
				return sendNumeric(c, ERR_USERNOTINCHANNEL, nick + " " + ch->getName() + " :They aren't on that channel");
			if (adding)
				ch->addOperator(target);
			else
				ch->removeOperator(target);
			continue;
		}
	}

	ch->broadcast(*this, ":" + c->prefix() + " MODE " + ch->getName() + " " + modeStr + params + "\r\n", NULL);
}
