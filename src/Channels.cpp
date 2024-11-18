#include "../inc/Channels.hpp"

Channels::Channels() {}
Channels::Channels(std::string channelName) {
	this->ChannelName = channelName;
	this->Public = true; // without invit
	this->Secret = false; // without key
    this->Topic = "";
	this->TopicSet = false;
	this->LimitSet = false;
	this->MemberLimit = 0;
}

Channels::~Channels() {}

std::string Channels::getChannelName() const {
    return ChannelName;
}

std::vector<int> Channels::getList(int ListType) const {
    switch (ListType) {
        case 0:
            return Members;
        case 1:
            return Operators;
        case 2:
            return Banned;
        default:
            return std::vector<int>();
    }
}

void Channels::setKey(const std::string& key) {
    Key = key;
}

void Channels::setTopic(const int clientId, const std::string& topic) {
    Topic = topic;
    TopicSetter = clientId;
    std::time_t current_time = std::time(NULL);
    TopicTime = std::ctime(&current_time); 
}

void Channels::setMemberLimit(int limit) {
    MemberLimit = limit;
}

void Channels::setTopicMode(bool mode) {
    this->TopicSet = mode;
}

void Channels::addClientIn(int Type, int client) {
    switch (Type) {
        case 0:
            Members.push_back(client);
            break;
        case 1:
            Operators.push_back(client);
            break;
        case 2:
            Banned.push_back(client);
            break;
    }
}

void Channels::removeClientFrom(int Type, int client) {
    std::vector<int>* list = nullptr;
    switch (Type) {
        case 0:
            list = &Members;
            break;
        case 1:
            list = &Operators;
            break;
        case 2:
            list = &Banned;
            break;
    }
    if (list) {
        list->erase(std::remove(list->begin(), list->end(), client), list->end());
    }
}

int Channels::getClientType(int client) const {
    if (std::find(Members.begin(), Members.end(), client) != Members.end()) {
        return 0;
    }
    if (std::find(Operators.begin(), Operators.end(), client) != Operators.end()) {
        return 1;
    }
    if (std::find(Banned.begin(), Banned.end(), client) != Banned.end()) {
        return 2;
    }
    return -1;
}

bool Channels::isPublic() const {
    return Public;
}

bool Channels::isSecret() const {
    return Secret;
}

bool Channels::isTopicSet() const {
    return TopicSet;
}

bool Channels::isLimitSet() const {
    return LimitSet;
}

void Channels::inviteClient(int target, int source) {
	InviteList[target] = source;
}

bool Channels::isInvited(int clientid) const {
	return InviteList.find(clientid) != InviteList.end();
}

void Channels::banClient(int clientid) {
	Banned.push_back(clientid);
}

void Channels::unbanClient(int clientid) {
	Banned.erase(std::remove(Banned.begin(), Banned.end(), clientid), Banned.end());
}

// Kick Client from Channel
void Channels::kickClient(int clientid) {
	Members.erase(std::remove(Members.begin(), Members.end(), clientid), Members.end());
	Operators.erase(std::remove(Operators.begin(), Operators.end(), clientid), Operators.end());
	Banned.erase(std::remove(Banned.begin(), Banned.end(), clientid), Banned.end());
	// Check if Client have invite someone to the channel Before Kicking him out
	for (std::map<int,int>::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
		if (it->second == clientid) {
			InviteList.erase(it);
			break;
		}
	}
}

bool Channels::isBanned(const int clientid) const {
    return std::find(Banned.begin(), Banned.end(), clientid) != Banned.end();
}

bool Channels::isOperator(const int clientid) const {
    return std::find(Operators.begin(), Operators.end(), clientid) != Operators.end();
}

bool Channels::isMember(const int clientid) const {
    return std::find(Members.begin(), Members.end(), clientid) != Members.end();
}

bool Channels::isJoined(const int clientid) const {
    return isMember(clientid) || isOperator(clientid);
}


bool Channels::isFull() const {
    return Members.size() >= CHANLIMIT;
}

bool Channels::isKeyValid(const std::string& key) const {
    return Key == key;
}

const std::string Channels::getTopic() const{
    return Topic;
}

std::string Channels::namReply() const{
    std::string clients;

    for (size_t i = 0; i < Members.size(); i++) {
        clients += DataControler::getClientNickname(Members[i]) + " ";
    }
    for (size_t i = 0; i < Operators.size(); i++) {
        clients += "@" + DataControler::getClientNickname(Operators[i]) + " ";
    }
    return clients;
}

const std::string Channels::getTopicSetter() const{
    std::string novalid = "No Longer Member";
    std::string nickname;
    if (TopicSetter == -1){
        return novalid;
    }
    nickname = DataControler::getClientNickname(TopicSetter);
    return nickname;
}

// (<setat> is a unix timestamp). Sent after
const std::string Channels::getTopicTime() const{
    return TopicTime;
}