//hamza

#include "Client.hpp"

Client::Client() : _fd(-1), _passwd(false), _nickSet(false), _userSet(false), _welcomed(false) {}

Client::Client(int fd, const std::string &host)
	: _fd(fd), _host(host), _passwd(false), _nickSet(false), _userSet(false), _welcomed(false) {}

Client::~Client() {}

int					Client::getFd() const			{ return _fd; }
const std::string	&Client::getNick() const 		{ return _nick; }
const std::string	&Client::getUser() const 		{ return _user; }
const std::string	&Client::getReal() const 		{ return _real; }
const std::string	&Client::getHost() const 		{ return _host; }
bool				Client::hasPasswd() const		{ return _passwd; }
bool				Client::hasNick() const			{ return _nickSet; }
bool				Client::hasUser() const			{ return _userSet; }
bool				Client::hasWelcomed() const		{ return _welcomed; }
bool				Client::isRegistered() const	{return _passwd && _nickSet && _userSet;}

void Client::setNick(const std::string &n)		{ _nick = n; _nickSet = true; }
void Client::setUser(const std::string &u) 		{ _user = u; _userSet = true; }
void Client::setReal(const std::string &r)		{ _real = r; }
void Client::setPasswd(bool v)					{ _passwd = v; }
void Client::setWelcomed(bool v)				{ _welcomed = v; }
void Client::appendBuf(const std::string &data)	{_buf += data;}

std::string Client::extractLine()
{
	size_t pos = _buf.find("\r\n");
	if (pos == std::string::npos)
		return "";
	std::string line = _buf.substr(0, pos);
	_buf.erase(0, pos + 2);
	return line;
}

void Client::appendOut(const std::string &data)
{
	_out += data;
}

const std::string &Client::getOut() const
{
	return _out;
}

void Client::consumeOut(size_t n)
{
	_out.erase(0, n);
}

bool Client::hasOut() const
{
	return !_out.empty();
}

std::string Client::prefix() const
{
	return _nick + "!" + _user + "@" + _host;
}
