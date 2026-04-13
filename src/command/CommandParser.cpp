//badreddine

#include "Server.hpp"

static std::vector<std::string> split(const std::string &s, char delim) 
{
	std::vector<std::string> tokens;
	std::istringstream iss(s);
	std::string token;
	while (std::getline(iss, token, delim)) {
		if (!token.empty())
			tokens.push_back(token);
	}
	return tokens;
}

static std::string toUpper(const std::string &s) 
{
	std::string r = s;
	for (size_t i = 0; i < r.size(); i++)
		r[i] = std::toupper(r[i]);
	return r;
}

void Server::handleMessage(Client *c, const std::string &line) 
{
	if (line.empty())
		return;

	std::string msg = line;

	if (msg[0] == ':') {
		size_t sp = msg.find(' ');
		if (sp == std::string::npos)
			return;
		msg = msg.substr(sp + 1);
	}

	std::string trailing;
	size_t colon = msg.find(" :");
	if (colon != std::string::npos) {
		trailing = msg.substr(colon + 2);
		msg = msg.substr(0, colon);
	}

	std::vector<std::string> args = split(msg, ' ');
	if (args.empty())
		return;

	std::string cmd = toUpper(args[0]);
	args.erase(args.begin());

	if (!trailing.empty())
		args.push_back(trailing);

	if (cmd == "PASS")    return cmdPass(c, args);
	if (cmd == "NICK")    return cmdNick(c, args);
	if (cmd == "USER")    return cmdUser(c, args);
	if (cmd == "QUIT")    return cmdQuit(c, args);
	if (cmd == "PING")    return cmdPing(c, args);
	if (cmd == "CAP" || cmd == "PONG")
		return;

	if (!c->isRegistered()) {
		sendNumeric(c, ERR_NOTREGISTERED, ":You have not registered");
		return;
	}

	if (cmd == "JOIN")    return cmdJoin(c, args);
	if (cmd == "PRIVMSG") return cmdPrivmsg(c, args);
	if (cmd == "KICK")    return cmdKick(c, args);
	if (cmd == "INVITE")  return cmdInvite(c, args);
	if (cmd == "TOPIC")   return cmdTopic(c, args);
	if (cmd == "MODE")    return cmdMode(c, args);
	if (cmd == "NAMES")   return cmdNames(c, args);

	sendNumeric(c, ERR_UNKNOWNCOMMAND, cmd + " :Unknown command");
}
