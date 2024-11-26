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

bool Clients::isClientInChannel(const std::string &channelName) const {
    std::string channelNameLower = DataControler::transformCase(channelName);
    return std::find(joinedChannels.begin(), joinedChannels.end(), channelNameLower) != joinedChannels.end();
}