//hamza

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Replies.hpp"

class	Server
{
	public:
		Server(int port, const std::string &password);
		~Server();

		void				run();
		void				sendReply(Client *c, const std::string &msg);
		void				sendNumeric(Client *c, const std::string &code, const std::string &params);
		Channel				*createChannel(const std::string &name);
		void				removeChannel(const std::string &name);
		Client				*findByNick(const std::string &nick);
		void				handleMessage(Client *c, const std::string &line);
		Channel				*getChannel(const std::string &name);
		const std::string	&getPassword() const;

	private:
		Server();
		Server(const Server &);
		Server &operator=(const Server &);

		void	initSocket();
		void	acceptClient();
		void	receiveData(int i);
		bool	flushWrite(int i);
		void	disconnectClient(int i);
		void	tryRegister(Client *c);
		void	cmdPass(Client *c, const std::vector<std::string> &args);
		void	cmdNick(Client *c, const std::vector<std::string> &args);
		void	cmdUser(Client *c, const std::vector<std::string> &args);
		void	cmdQuit(Client *c, const std::vector<std::string> &args);
		void	cmdPing(Client *c, const std::vector<std::string> &args);
		void	cmdJoin(Client *c, const std::vector<std::string> &args);
		void	cmdPrivmsg(Client *c, const std::vector<std::string> &args);
		void	cmdKick(Client *c, const std::vector<std::string> &args);
		void	cmdInvite(Client *c, const std::vector<std::string> &args);
		void	cmdTopic(Client *c, const std::vector<std::string> &args);
		void	cmdMode(Client *c, const std::vector<std::string> &args);
		void	cmdNames(Client *c, const std::vector<std::string> &args);

		int								_port;
		std::string						_password;
		int								_listenFd;
		std::vector<struct pollfd>		_pollfds;
		std::map<int, Client*>			_clients;
		std::map<std::string, Channel*>	_channels;
};

#endif
