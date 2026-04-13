//abderahman

#include "Server.hpp"
void Server::disconnectClient(int i) 
{
	int fd = _pollfds[i].fd;
	std::map<int, Client*>::iterator itClient = _clients.find(fd);
	Client *c = NULL;
	if (itClient != _clients.end())
		c = itClient->second;

	std::cout << "Client disconnected: ";
	if (c)
		std::cout << c->getNick();
	std::cout << " (fd " << fd << ")" << std::endl;

	if (c)
	{
		for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ) 
		{
			Channel *ch = it->second;
			ch->removeInvite(c);
			if (ch->isMember(c)) 
			{
				ch->broadcast(*this, ":" + c->prefix() + " QUIT :Disconnected\r\n", c);
				ch->removeMember(c);
			}
			if (ch->memberCount() == 0) 
			{
				delete ch;
				_channels.erase(it++);
			} 
			else
				++it;
		}
	}

	close(fd);
	if (c)
		delete c;
	_clients.erase(fd);
	_pollfds.erase(_pollfds.begin() + i);
}
