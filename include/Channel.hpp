//abderahman

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Client;
class Server;

class Channel 
{
public:
	Channel();
	Channel(const std::string &name);
	~Channel();

	const std::string          &getName() const;
	const std::string          &getTopic() const;
	void setTopic(const std::string &t);
	void                 addMember(Client *c);
	void                 removeMember(Client *c);
	bool                 isMember(Client *c) const;
	size_t               memberCount() const;
	std::vector<Client*> getMembers() const;
	void  addOperator(Client *c);
	void  removeOperator(Client *c);
	bool  isOperator(Client *c) const;
	bool  isInviteOnly() const;
	void  setInviteOnly(bool v);
	bool  isTopicRestricted() const;
	void  setTopicRestricted(bool v);
	bool  hasKey() const;
	const std::string &getKey() const;
	void  setKey(const std::string &key);
	void  clearKey();
	bool  hasLimit() const;
	size_t getLimit() const;
	void  setLimit(size_t limit);
	void  clearLimit();
	void  invite(Client *c);
	bool  isInvited(Client *c) const;
	void  removeInvite(Client *c);
	void  broadcast(Server &server, const std::string &msg, Client *except);

private:
	std::string          _name;
	std::string          _topic;
	std::vector<Client*> _members;
	std::set<Client*>    _operators;
	bool                 _inviteOnly;
	bool                 _topicRestricted;
	bool                 _hasKey;
	std::string          _key;
	bool                 _hasLimit;
	size_t               _limit;
	std::set<Client*>    _invited;
};

#endif
