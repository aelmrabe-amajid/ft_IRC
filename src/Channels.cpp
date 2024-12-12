#include "../inc/Channels.hpp"

Channels::Channels() {}
Channels::Channels(std::string channelName) {
	this->ChannelName = channelName;
	this->Public = true;
	this->Secret = false;
    this->Topic = "";
	this->TopicSet = false;
	this->LimitSet = false;
	this->MemberLimit = 0;
    this->InviteList = std::map<int,std::vector<int> >();
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
    time_t now;
    time(&now);
    TopicTime = std::to_string(now);
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
    std::vector<int>* list;
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


bool Channels::isInvited(int clientid){
    for (std::map<int,std::vector<int> >::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
        for(std::vector<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            if (*it2 == clientid)
                return true;
        }
    }
	return (false);
}

void Channels::banClient(int clientid) {
	Banned.push_back(clientid);
}

void Channels::unbanClient(int clientid) {
	Banned.erase(std::remove(Banned.begin(), Banned.end(), clientid), Banned.end());
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
    int MemberNum = static_cast<int>(Members.size()) + static_cast<int>(Operators.size());
    if (this->MemberLimit == 0)
        return false;
    if (MemberNum + 1 > this->MemberLimit)
        return (true);
    return (false);
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

const std::string Channels::getTopicTime() const{
    return TopicTime;
}


std::string Channels::getMemberList(){
    std::string clients = "";
    for (size_t i = 0; i < Operators.size(); i++)
        clients += "@" + DataControler::getClientNickname(Operators[i]) + " ";
    for (size_t i = 0; i < Members.size(); i++)
        clients += DataControler::getClientNickname(Members[i]) + " ";
    return clients;
}

std::string Channels::getChannelModes(){
    std::string modes = "";
    if (isPublic() == false)
        modes += "i";
    if (isSecret())
        modes += "k";
    if (isTopicSet())
        modes += "t";
    if (isLimitSet())
        modes += "l ";
    if (modes.empty() == false)
        modes = "+" + modes;
    return modes;
}

void Channels::setChannelMode(char mode, bool set){
    if (mode == 'i') {
        if(!set){
            if (Public == false)
                RmInvite(0,0,3);
            Public = true;
        }
        else if (set){
            Public = false;
        }
        set ? Public = false : Public = true;
    } else if (mode == 't') {
        set ? TopicSet = true : TopicSet = false;
    }
}

void Channels::setChannelMode(char mode, bool set, std::string value){
    if (mode == 'k') {
        set ? (Secret = true, Key = value) : (Secret = false, Key = "");
    } else if (mode == 'l') {
        set ? (LimitSet = true, MemberLimit = atoi(value.c_str())) : (LimitSet = false, MemberLimit = 0);
    }
}


int Channels::getMemberLimit() const {
    return (this->MemberLimit);
}

int Channels::getMemberCount() const {
    int OperatorsCount = getList(1).size();
    int MembersCount = getList(0).size();
    return (OperatorsCount + MembersCount);
}



void Channels::AddInvite(int inviter, int invited) {
    if (InviteList.find(inviter) == InviteList.end())
        InviteList[inviter] = std::vector<int>();
    if (std::find(InviteList[inviter].begin(), InviteList[inviter].end(), invited) == InviteList[inviter].end()) {
        InviteList[inviter].push_back(invited);
        Clients *cl = DataControler::getClient(invited);
        cl->PendingInvite(this->ChannelName, inviter, 1);
    }
}

void Channels::RmInvite(int inviter, int invited, int type) {
    if (type == 1 || type == 0) {
        for (std::map<int, std::vector<int> >::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
            std::vector<int>& invitedList = it->second;
            invitedList.erase(std::remove(invitedList.begin(), invitedList.end(), invited), invitedList.end());
        }
        if (type == 1) {
            Clients *cl = DataControler::getClient(invited);
            cl->PendingInvite(this->ChannelName, 0, 2);
        }
    } else if (type == 2) {
        if (InviteList.find(inviter) != InviteList.end()) {
            Clients *cl;
            std::vector<int>& invitedList = InviteList[inviter];
            for (std::vector<int>::iterator it = invitedList.begin(); it != invitedList.end(); ++it) {
                cl = DataControler::getClient(*it);
                cl->PendingInvite(ChannelName, inviter, 4);
            }
            InviteList.erase(inviter);
        }
    } else if (type == 3) {
        std::set<int> InvitedList;
        for (std::map<int, std::vector<int> >::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
            std::vector<int>& invitedList = it->second;
            InvitedList.insert(invitedList.begin(), invitedList.end());
        }
        if (!InvitedList.empty()) {
            Clients *cl;
            for (std::set<int>::iterator it = InvitedList.begin(); it != InvitedList.end(); ++it) {
                cl = DataControler::getClient(*it);
                cl->PendingInvite(ChannelName, 0, 3);
            }
        }
    }
}
