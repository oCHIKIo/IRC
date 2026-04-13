//badreddine

#include "Server.hpp"

Server::Server(int port, const std::string &password)
	: _port(port), _password(password), _listenFd(-1)
{
	initSocket();
	std::cout << "Server listening on port " << _port << std::endl;
}
Server::~Server()
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second)
		{
			int fd = it->second->getFd();
			if (fd >= 0)
				close(fd);
			delete it->second;
		}
	}
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	if (_listenFd >= 0)
		close(_listenFd);
}

const std::string &Server::getPassword() const { return _password; }
void Server::sendReply(Client *c, const std::string &msg) 
{
	if (!c)
		return;
	c->appendOut(msg);
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == c->getFd())
		{
			_pollfds[i].events |= POLLOUT;
			break;
		}
	}
}
void Server::sendNumeric(Client *c, const std::string &code, const std::string &params) 
{
	std::string nick;
	if (c->getNick().empty())
		nick = "*";
	else
		nick = c->getNick();
	std::string msg = ":ircserv " + code + " " + nick + " " + params + "\r\n";
	sendReply(c, msg);
}
Channel *Server::getChannel(const std::string &name) 
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}
Channel *Server::createChannel(const std::string &name) 
{
	Channel *ch = new Channel(name);
	_channels[name] = ch;
	return ch;
}

void Server::removeChannel(const std::string &name) 
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end()) 
    {
		delete it->second;
		_channels.erase(it);
	}
}
Client *Server::findByNick(const std::string &nick) 
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (it->second->getNick() == nick)
			return it->second;
	return NULL;
}
void Server::tryRegister(Client *c) 
{
	if (!c->isRegistered() || c->hasWelcomed())
		return;
	sendNumeric(c, RPL_WELCOME, ":Welcome to the IRC server, " + c->prefix());
	sendNumeric(c, RPL_YOURHOST, ":Your host is ircserv, running version 1.0");
	sendNumeric(c, RPL_CREATED, ":This server was created today");
	c->setWelcomed(true);
}
