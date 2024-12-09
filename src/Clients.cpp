#include "../inc/Clients.hpp"
#include "../inc/DataControler.hpp"
#include "../inc/Macros.hpp"

Clients::Clients() : socketfd(-1) {};
Clients::Clients(int socketfd) : socketfd(socketfd) {};

Clients::~Clients() {}

std::string Clients::getHostName() const {
    return hostName;
}

std::string Clients::getNickName() const {
    return nickName;
}

std::string Clients::getUserName() const {
    return userName;
}

std::string Clients::getRealName() const {
    return realName;
}

int Clients::getID() const {
    return socketfd;
}

std::vector<std::string> Clients::getJoinedChannels() const {
    return joinedChannels;
}

void Clients::setRegistrationStatus(int status) {
    isRegistred = status;
}

void Clients::setNickName(const std::string &nickname) {
    if (nickname.length() > 9)
        nickName = nickName.substr(0, 9);
    if (isRegistred == 1)
        DataControler::modifyClientNickname(socketfd, nickname);
    else
        DataControler::addNicknames(nickname, socketfd);
    nickName = nickname;
}

void Clients::setUserName(const std::string &username) {
    this->userName = username;
}

void Clients::setHostName(const std::string &hostname) {
    this->hostName = hostname;
}

void Clients::setRealName(const std::string &realname) {
    this->realName = realname;
}

int Clients::getRegistrationStatus() const {
    return this->isRegistred;
}

void Clients::joinChannel(const std::string &channelName) {
    std::string channelNameLower = DataControler::transformCase(channelName);
    joinedChannels.push_back(channelNameLower);
}

void Clients::leaveChannel(const std::string &channelName) {
    std::string channelNameLower = DataControler::transformCase(channelName);
    joinedChannels.erase(std::remove(joinedChannels.begin(), joinedChannels.end(), channelNameLower), joinedChannels.end());
}

void Clients::leaveChannel(void){
    std::vector<std::string> chan_list = this->getJoinedChannels();
    std::string channel_name;
    Command *cmd;
    for (std::vector<std::string>::iterator it = chan_list.begin(); it != chan_list.end(); it++){
        if ((*it).empty() == false){
            channel_name = "#" + *it;
            cmd = Command::createCommand(this->socketfd, channel_name, PART);
            cmd->execute();
            delete cmd;
        }
    }
}

bool Clients::isClientInChannel(const std::string &channelName) const {
    std::string channelNameLower = DataControler::transformCase(channelName);
    return std::find(joinedChannels.begin(), joinedChannels.end(), channelNameLower) != joinedChannels.end();
}

std::map<std::string, std::vector<int> > Clients::getRecivedInvites(void){
    return (this->RecivedInvites);
}

/*
    type 1 : getting invite to channel from clientID 
    type 2 : joining channel and then remove all pending invites
    type 3 : channel was deleted so delete all pending invites 
    type 4 : remove pedding invite from joining channnelname from this client ( in cas inviter leave or downgraded)
    type 5 : remove all pendding invite from this client
*/
void Clients::PendingInvite(std::string channel_name, int ClientID, int type){
    if (type == 1) {
        if (RecivedInvites.find(channel_name) != RecivedInvites.end()){
            RecivedInvites[channel_name].push_back(ClientID);
        }
    }
    else if (type == 2 || type == 3){
        if (RecivedInvites.find(channel_name) != RecivedInvites.end()){
            RecivedInvites.erase(channel_name);
        }
    }
    else if (type == 4){
        if (RecivedInvites.find(channel_name) != RecivedInvites.end()){
            std::vector<int> Inviters = RecivedInvites[channel_name];
            for (std::vector<int>::iterator it = Inviters.begin(); it != Inviters.end(); it++){
                if (*it == ClientID)
                    Inviters.erase(it);
            }
        }
    }
    else if (type == 5 && RecivedInvites.size()){
        Channels *ch;
        for (std::map<std::string, std::vector<int> >::iterator it = RecivedInvites.begin(); it != RecivedInvites.end(); it++){
            ch = DataControler::getChannel(it->first);
            ch->RmInvite(0,socketfd,0);
        }
    }
}