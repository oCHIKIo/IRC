//hamza
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class	Client
{
	private:
		int			_fd;
		std::string _nick;
		std::string _user;
		std::string _real;
		std::string _host;
		bool		_passwd;
		bool		_nickSet;
		bool		_userSet;
		std::string _buf;
		std::string _out;
		bool		_welcomed;

	public:
		Client();
		Client(int fd, const std::string &host);
		~Client();

		int					getFd() const;
		const std::string	&getNick() const;
		const std::string	&getUser() const;
		const std::string	&getReal() const;
		const std::string	&getHost() const;
		const std::string	&getOut() const;

		void				setNick(const std::string &n);
		void				setUser(const std::string &u);
		void				setReal(const std::string &r);
		void				setPasswd(bool v);
		void				setWelcomed(bool v);

		bool				hasPasswd() const;
		bool				hasNick() const;
		bool				hasUser() const;
		bool				hasWelcomed() const;
		bool				hasOut() const;

		std::string			extractLine();
		void				appendOut(const std::string &data);
		bool				isRegistered() const;
		void				appendBuf(const std::string &data);
		void				consumeOut(size_t n);
		std::string 		prefix() const;

};

#endif
