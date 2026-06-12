//badreddine

#include "Server.hpp"
void Server::initSocket() 
{
	_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenFd < 0)
		throw std::runtime_error("socket() failed");

	int opt = 1;
	setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	fcntl(_listenFd, F_SETFL, O_NONBLOCK);

	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);

	if (bind(_listenFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind() failed");
	if (listen(_listenFd, MAX_CLIENTS) < 0)
		throw std::runtime_error("listen() failed");

	struct pollfd pfd;
	pfd.fd = _listenFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
}
void Server::acceptClient() 
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int fd = accept(_listenFd, (struct sockaddr*)&addr, &len);
	if (fd < 0)
		return;

	fcntl(fd, F_SETFL, O_NONBLOCK);

	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);

	std::string host = inet_ntoa(addr.sin_addr);
	_clients[fd] = new Client(fd, host);
	std::cout << "New connection from " << host << " (fd " << fd << ")" << std::endl;
}
void Server::run() 
{
	while (g_running)
    {
		int ret = poll(&_pollfds[0], _pollfds.size(), -1);
		if (ret < 0) 
        {
			if (errno == EINTR)
				continue;
			throw std::runtime_error("poll() failed");
		}
		for (size_t i = 0; i < _pollfds.size(); i++) 
        {
			if (_pollfds[i].revents == 0)
				continue;
			int fd = _pollfds[i].fd;
			if (_pollfds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) 
            {
				if (fd == _listenFd)
					throw std::runtime_error("listen socket error");
				disconnectClient(i);
				i--;
				continue;
			}
			if (fd == _listenFd)
            {
				if (_pollfds[i].revents & POLLIN)
					acceptClient();
				continue;
			}
			if (_pollfds[i].revents & POLLIN) 
			{
				receiveData(i);
				if (i >= _pollfds.size() || _pollfds[i].fd != fd)
				{
					i--;
					continue;
				}
			}
			if (_pollfds[i].revents & POLLOUT)
			{
				if (flushWrite(i))
				{
					i--;
					continue;
				}
			}
		}
	}
}

/*
	--> 

*/
