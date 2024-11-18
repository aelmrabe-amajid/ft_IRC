#include "../inc/Clients.hpp"
#include "../inc/DataControler.hpp"
#include "../inc/Macros.hpp"

Clients::Clients() : socketfd(-1), isRegistred(0) {}
Clients::Clients(int socketfd) : socketfd(socketfd), isRegistred(0) {}

Clients::~Clients() {}

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
    if (isRegistred == 1){
        nickName = nickname;
        userName = nickname;
        realName = nickname;
        isRegistred = 2;
    }else if (isRegistred == 2){
        DataControler::modifyClientNickname(socketfd, nickname);
        nickName = nickname;
        userName = nickname;
        realName = nickname;
    }else {
        DataControler::modifyClientNickname(socketfd, nickname);
        nickName = nickname;
    }
}

void Clients::setUserName(const std::string &username) {
    this->userName = username;
}

void Clients::setRealName(const std::string &realname) {
    this->realName = realname;
}

int Clients::getRegistrationStatus() const {
    return this->isRegistred;
}

void Clients::joinChannel(const std::string &channelName) {
    joinedChannels.push_back(channelName);
}

void Clients::leaveChannel(const std::string &channelName) {
    joinedChannels.erase(std::remove(joinedChannels.begin(), joinedChannels.end(), channelName), joinedChannels.end());
}

bool Clients::isClientInChannel(const std::string &channelName) const {
    return std::find(joinedChannels.begin(), joinedChannels.end(), channelName) != joinedChannels.end();
}