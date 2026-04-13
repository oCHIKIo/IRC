//abderahman

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

Channel::Channel() : _inviteOnly(false), _topicRestricted(false), _hasKey(false), _hasLimit(false), _limit(0) {}

Channel::Channel(const std::string &name)
	: _name(name), _inviteOnly(false), _topicRestricted(false), _hasKey(false), _hasLimit(false), _limit(0) {}

Channel::~Channel() {}

const std::string &Channel::getName() const { return _name; }
const std::string &Channel::getTopic() const { return _topic; }
void Channel::setTopic(const std::string &t) { _topic = t; }
void Channel::addMember(Client *c) 
{
	if (!isMember(c))
		_members.push_back(c);
	_invited.erase(c);
}

void Channel::removeMember(Client *c) 
{
	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) 
    {
		if (*it == c) 
        { 
            _members.erase(it); 
            break; 
        }
	}
	_operators.erase(c);
	_invited.erase(c);
}

bool Channel::isMember(Client *c) const 
{
	for (size_t i = 0; i < _members.size(); i++)
		if (_members[i] == c) return true;
	return false;
}

size_t Channel::memberCount() const { return _members.size(); }
std::vector<Client*> Channel::getMembers() const { return _members; }

void Channel::addOperator(Client *c) { _operators.insert(c); }
void Channel::removeOperator(Client *c) { _operators.erase(c); }
bool Channel::isOperator(Client *c) const { return _operators.count(c) > 0; }

bool Channel::isInviteOnly() const { return _inviteOnly; }
void Channel::setInviteOnly(bool v) { _inviteOnly = v; }

bool Channel::isTopicRestricted() const { return _topicRestricted; }
void Channel::setTopicRestricted(bool v) { _topicRestricted = v; }

bool Channel::hasKey() const { return _hasKey; }
const std::string &Channel::getKey() const { return _key; }
void Channel::setKey(const std::string &key) { _key = key; _hasKey = true; }
void Channel::clearKey() { _key.clear(); _hasKey = false; }

bool Channel::hasLimit() const { return _hasLimit; }
size_t Channel::getLimit() const { return _limit; }
void Channel::setLimit(size_t limit) { _limit = limit; _hasLimit = true; }
void Channel::clearLimit() { _limit = 0; _hasLimit = false; }

void Channel::invite(Client *c) { _invited.insert(c); }
bool Channel::isInvited(Client *c) const { return _invited.count(c) > 0; }
void Channel::removeInvite(Client *c) { _invited.erase(c); }
void Channel::broadcast(Server &server, const std::string &msg, Client *except) 
{
	for (size_t i = 0; i < _members.size(); i++) 
    {
		if (_members[i] != except)
			server.sendReply(_members[i], msg);
	}
}
