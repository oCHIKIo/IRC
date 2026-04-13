//hamza

#include "Server.hpp"

void Server::cmdPrivmsg(Client *c, const std::vector<std::string> &args)
{
	if (args.size() < 2)
		return sendNumeric(c, ERR_NEEDMOREPARAMS, "PRIVMSG :Not enough parameters");

	const std::string &target = args[0];
	const std::string &text = args[1];

	if (!target.empty() && target[0] == '#')
	{
		Channel *ch = getChannel(target);
		if (!ch)
			return sendNumeric(c, ERR_NOSUCHCHANNEL, target + " :No such channel");
		if (!ch->isMember(c))
			return sendNumeric(c, ERR_CANNOTSENDTOCHAN, target + " :Cannot send to channel");
		ch->broadcast(*this, ":" + c->prefix() + " PRIVMSG " + target + " :" + text + "\r\n", c);
		return;
	}

	Client *to = findByNick(target);
	if (!to)
		return sendNumeric(c, ERR_NOSUCHNICK, target + " :No such nick");
	sendReply(to, ":" + c->prefix() + " PRIVMSG " + target + " :" + text + "\r\n");
}
