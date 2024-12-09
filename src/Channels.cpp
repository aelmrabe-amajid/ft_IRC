#include "../inc/Channels.hpp"

Channels::Channels() {}
Channels::Channels(std::string channelName) {
	this->ChannelName = channelName;
	this->Public = true; // 
	this->Secret = false; // without key
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

// void Channels::inviteClient(int target, int source) {
// 	InviteList[target] = source;
// }

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

// Kick Client from Channel
// void Channels::kickClient(int clientid) {
// 	Members.erase(std::remove(Members.begin(), Members.end(), clientid), Members.end());
// 	Operators.erase(std::remove(Operators.begin(), Operators.end(), clientid), Operators.end());
// 	Banned.erase(std::remove(Banned.begin(), Banned.end(), clientid), Banned.end());
// 	// Check if Client have invite someone to the channel Before Kicking him out
// 	for (std::map<int,int>::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
// 		if (it->second == clientid) {
// 			InviteList.erase(it);
// 			break;
// 		}
// 	}
// }

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

// (<setat> is a unix timestamp). Sent after
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
        set ? (LimitSet = true, MemberLimit = std::stoi(value)) : (LimitSet = false, MemberLimit = 0);
    }
}


int Channels::getMemberLimit() const {
    return (this->MemberLimit);
}

int Channels::getMemberCount() const {
    return (this->MemberCount);
}

// std::vector<int> Channels::PendingInvitesFrom(int clientid) {
//     std::vector<int> invites;
//     for (std::map<int,int>::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
//         if (it->second == clientid) {
//             invites.push_back(it->first);
//         }
//     }
//     return invites;
// }

// void Channels::RemovePendingInvitesFrom(int clientid) {
//     for (std::map<int,int>::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
//         if (it->second == clientid)
//             InviteList.erase(it);
//     }
// }

// void Channels::RemovePendingInvitesFromMembers() {
//     std::vector<int> operators = getList(1);
//     for (std::map<int,int>::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
//         if (std::find(operators.begin(), operators.end(), it->second) == operators.end())
//             InviteList.erase(it);
//     }
// }

// void Channels::InviteAccepted(int clientid) {
//     for (std::map<int,int>::iterator it = InviteList.begin(); it != InviteList.end(); ++it) {
//         if (it->first == clientid) {
//             InviteList.erase(it);
//             break;
//         }
//     }
// }

// void Channels::addInvite(int invited, int inviter) {
//     InviteList[invited] = inviter;
// }

// std::map<int,std::vector<int> > Channels::getInviteMap(void){
//     return (this->InviteList);
// }


/*
    Inviter -> vector of Invites
*/
// void Channels::addInvite(int inviter, int invited){
//     this->InviteList[inviter].push_back(invited);
// }



void Channels::AddInvite(int inviter, int invited) {
    if (InviteList.find(inviter) == InviteList.end())
        InviteList[inviter] = std::vector<int>();
    if (std::find(InviteList[inviter].begin(), InviteList[inviter].end(), invited) == InviteList[inviter].end()) {
        InviteList[inviter].push_back(invited);
        Clients *cl = DataControler::getClient(invited);
        cl->PendingInvite(this->ChannelName, inviter, 1);
    }
}

// void Channels::AddInvite(int inviter, int invited){
//     if (InviteList.find(inviter) != InviteList.end()){
//         for (std::vector<int>::iterator it = InviteList[inviter].begin(); it != InviteList[inviter].end(); it++){
//             if (*it == invited)
//                 return;
//         }
//     }
//     else
//         InviteList[inviter].push_back(invited);
//     Clients *cl = DataControler::getClient(invited);
//     cl->PendingInvite(this->ChannelName,inviter,1);
// };

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



/*
    type 1 : when client invited Join Channel (remove all pending invites) | when client invited quit server 

    Invited Case:
        Accept and Join
        QUIT Server
    Inviter Case:
        DownGrad From Operator
        leave Server
    ChannelDeleted:
*/

// void Channels::RmInvite(int inviter, int invited, int type){
//     if (type == 1 || type == 0){
//         for (std::map<int,std::vector<int> >::iterator it = InviteList.begin(); it !=  InviteList.end(); it++){
//             for (std::vector<int>::iterator v_it = it->second.begin(); v_it != it->second.end(); it++){
//                 if (*v_it == invited)
//                     it->second.erase(v_it);
//             }
//         }
//         if (type == 1){
//             Clients *cl = DataControler::getClient(invited);
//             cl->PendingInvite(this->ChannelName,0,2);
//         }
//     }
//     else if (type == 2){
//         if (InviteList.find(inviter) != InviteList.end()){
//             Clients *cl;
//             for(std::vector<int>::iterator it = InviteList[inviter].begin(); it != InviteList[inviter].end(); it++){
//                 cl = DataControler::getClient(*it);
//                 cl->PendingInvite(ChannelName,inviter,4);
//             }
//             InviteList.erase(inviter);
//         }
//     }
//     else if (type == 3){
//         std::set<int> InvitedList;
//         for (std::map<int,std::vector<int> >::iterator it = InviteList.begin(); it !=  InviteList.end(); it++){
//             std::vector<int> &invitedList = it->second;
//             for (std::vector<int>::iterator v_it = invitedList.begin(); v_it != invitedList.end(); it++){
//                 InvitedList.insert(*v_it);
//             }
//         }
//         if (InvitedList.size()){
//             Clients *cl;
//             for(std::set<int>::iterator it = InvitedList.begin(); it != InvitedList.end(); it++){
//                 cl = DataControler::getClient(*it);
//                 cl->PendingInvite(ChannelName,0,3);
//             }
//         }
//     }
// }



// void Channels::RemovePendingInvite(int type, int ID){
//     // Sender
//     if (type == 1){
//         for (std::vector<int>::iterator it = this->Invites.Inviter.begin(); it != this->Invites.Inviter.end(); it++)
//         {
//             if ()
//         }
//     }
// };