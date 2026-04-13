//hamza

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Client 
{
private:
        int         _fd;
        std::string _nick;
        std::string _user;
        std::string _real;
        std::string _host;
        bool        _passwd;
        bool        _nickSet;
        bool        _userSet;
        std::string _buf;
	std::string _out;
	bool        _welcomed;
public:
	Client();
	Client(int fd, const std::string &host);
	~Client();

	int                getFd() const;
	const std::string &getNick() const;
	const std::string &getUser() const;
	const std::string &getReal() const;
	const std::string &getHost() const;
	bool               isRegistered() const;
	bool               hasPasswd() const;
	bool               hasNick() const;
	bool               hasUser() const;
	bool               hasWelcomed() const;
	void setNick(const std::string &n);
	void setUser(const std::string &u);
	void setReal(const std::string &r);
	void setPasswd(bool v);
	void setWelcomed(bool v);
	void               appendBuf(const std::string &data);
	std::string        extractLine();
	void               appendOut(const std::string &data);
	const std::string &getOut() const;
	void               consumeOut(size_t n);
	bool               hasOut() const;
	std::string prefix() const;

};

#endif
