//badreddine

#include "Server.hpp"
void Server::receiveData(int i) 
{
	char buf[BUFFER_SIZE];
	int fd = _pollfds[i].fd;
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it == _clients.end() || !it->second) { disconnectClient(i); return; }
	Client *c = it->second;

	ssize_t n = recv(fd, buf, sizeof(buf) - 1, 0);
	if (n < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		disconnectClient(i);
		return;
	}
	if (n == 0) { disconnectClient(i); return; }
	buf[n] = '\0';
	c->appendBuf(std::string(buf, n));

	std::string line;
	while (!(line = c->extractLine()).empty()) 
    {
		handleMessage(c, line);
		if (_clients.find(fd) == _clients.end())
			return;
	}
}

bool Server::flushWrite(int i)
{
	int fd = _pollfds[i].fd;
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it == _clients.end()) { _pollfds[i].events &= ~POLLOUT; return false; }
	Client *c = it->second;
	if (!c->hasOut()) { _pollfds[i].events &= ~POLLOUT; return false; }
	const std::string &out = c->getOut();
	ssize_t n = send(fd, out.c_str(), out.size(), 0);
	if (n < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false;
		disconnectClient(i);
		return true;
	}
	if (n == 0) { disconnectClient(i); return true; }
	c->consumeOut(static_cast<size_t>(n));
	if (!c->hasOut())
		_pollfds[i].events &= ~POLLOUT;
	return false;
}
