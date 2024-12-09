#include "../inc/DataControler.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <set>
#include <string>
#include <ctime>

#define BF_SIZE 1024

std::map<int, Clients> DataControler::clientslist;
std::map<std::string, Channels> DataControler::channelslist;
std::map<std::string, int> DataControler::nicknames;
std::string DataControler::password;
std::string DataControler::srv_date;

void DataControler::clearData() {
    for (std::map<int, Clients>::iterator it = clientslist.begin(); it != clientslist.end(); ++it)
        close(it->first);
    channelslist.clear();
    nicknames.clear();
}

void DataControler::initData(std::string psw) {
    time_t now = time(0);
    DataControler::srv_date = ctime(&now);
    DataControler::srv_date.pop_back();
    DataControler::password = psw;
    // RFC::INIT();
}

std::string DataControler::serverCreationDate() {
    return srv_date;
}

std::string DataControler::transformCase(const std::string& str) {
    std::string result;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        result += static_cast<char>(std::tolower(*it));
    }
    return result;
}


Clients* DataControler::getClient(int fd) {
    return &(clientslist[fd]);
}

Channels* DataControler::getChannel(const std::string& channelname) {
    std::string ch_lower = transformCase(channelname);
    return &(channelslist[ch_lower]);
}

/*
    * This function is safe to use because we will check if the client exists before calling it
*/
Clients* DataControler::getClient(const std::string& nickname) {
    std::string nick_lower = transformCase(nickname);
    return(&(clientslist[nicknames[nick_lower]]));
}


/*
    * This function will just iterate over the channelsName vector and get the members of each channel
    * and insert them into a 
*/
std::vector<int>& DataControler::getChannelsMembers(const std::vector<std::string>& channelsName) {
    static std::vector<int> members;
    members.clear();

    std::set<int> uniqueMembers;
    for (std::vector<std::string>::const_iterator it = channelsName.begin(); it != channelsName.end(); ++it) {
        Channels* channel = getChannel(*it);
        std::vector<int> channelMembers = channel->getList(0);
        uniqueMembers.insert(channelMembers.begin(), channelMembers.end());
    }
    members.assign(uniqueMembers.begin(), uniqueMembers.end());
    // members.push_back()
    return members;
}

bool DataControler::isClient(int fd) {
    return clientslist.find(fd) != clientslist.end();
}

bool DataControler::isClient(const std::string& nickname) {
    std::string nick_lower = transformCase(nickname);
    for (std::map<int, Clients>::iterator it = clientslist.begin(); it != clientslist.end(); ++it) {
        if (it->second.getNickName() == nick_lower) {
            return true;
        }
    }
    return false;
}

bool DataControler::isChannel(const std::string& channelname) {
    std::string ch_lower = transformCase(channelname);
    return channelslist.find(ch_lower) != channelslist.end();
}

bool DataControler::nicknamesUnique(const std::string& nickname) {
    std::string nick_lower = transformCase(nickname);
    if (nicknames.find(nick_lower) != nicknames.end())
        return false;
    return true;
}

std::string DataControler::getClientNickname(int fd){
    return clientslist[fd].getNickName();
}

bool DataControler::channelnamesExist(const std::string& channelname) {
    std::string ch_lower = transformCase(channelname);
    // i need to check this late
    // if (ch_lower.size() > CHANNELLEN)
    //     return (false);
    if (channelslist.find(ch_lower) != channelslist.end())
        return true; 
    return false;
}


void DataControler::addClient(int fd, const std::string& _hostname) {
    Clients cl = Clients(fd);
    cl.setNickName("");
    cl.setUserName("");
    cl.setRealName("");
    cl.setHostName(_hostname);
    cl.setRegistrationStatus(0);
    clientslist[fd] = cl;
}

void DataControler::addNicknames(const std::string& nickname, int clientID) {
    std::string nickname_lower = transformCase(nickname);
    nicknames[nickname_lower] = clientID;
}

void DataControler::modifyClientNickname(int fd, const std::string& newNickname)
{
    std::string nickname_lower = transformCase(newNickname);
    if (clientslist.find(fd) != clientslist.end()){
        nickname_lower = clientslist[fd].getNickName();
        nicknames.erase(nickname_lower);
    }
    nickname_lower = transformCase(newNickname);
    nicknames[nickname_lower] = fd;
}

void DataControler::removeClient(int fd, int flag) {
    if (DataControler::isClient(fd) == false)
        return;
    Clients *_cl = DataControler::getClient(fd);
    if (_cl->getRegistrationStatus() == 0){
        if (_cl->getNickName() != ""){
            std::string nickname_lower = transformCase(_cl->getNickName());
            nicknames.erase(nickname_lower);
        }
        clientslist.erase(fd);
        close(fd);
        return;
    }
    if (flag){
        Command *cmd = Command::createCommand(fd,"leaving...",QUIT);
        cmd->execute();
        delete cmd;
    }
    std::string nickname_lower = transformCase(_cl->getNickName());
    nicknames.erase(nickname_lower);
    clientslist.erase(fd);
    close(fd);
    return;
}

Channels *DataControler::addChannel(const std::string& channelname) {
    Channels channel = Channels(channelname);
    std::string channelname_lower = transformCase(channelname);
    channelslist[channelname_lower] = channel;
    return &(channelslist[channelname_lower]);
}

void DataControler::removeChannel(const std::string& channelname) {
    std::string channelname_lower = transformCase(channelname);
    channelslist.erase(channelname_lower);
}

bool DataControler::PasswordCheck(const std::string& psw) {
    return DataControler::password == psw;
}

void DataControler::SendClientMessage(int socketfd, const std::string &message) {
    send(socketfd, message.c_str(), message.size(), 0);
};

std::string DataControler::UPREF(const std::string& nick) {
    return ":" + nick + "!" + DataControler::getClient(nick)->getUserName() + "@" + DataControler::getClient(nick)->getHostName();
}

void DataControler::SendMsg(int clientid, std::string msg) {
    size_t len = msg.size();
    if (len > 1024)
        std::cerr << "Warning: Message is too long" << std::endl;
    if (len > 0 && send(clientid, msg.c_str(), len, 0) == -1)
        std::cerr << "Error sendMsg(): " << strerror(errno) << std::endl;
    std::cout << "\033[" << 32 << "m" << "[outgoing]: " << msg << "\033[0m";
}

void DataControler::SendMsg(const std::string &channelname, std::string msg) {
    Channels *ch = DataControler::getChannel(channelname);
    std::vector<int> o_clts = ch->getList(0);
    std::vector<int> clts = ch->getList(1);
    std::set<int> uniqueMembers;
    for (std::vector<int>::iterator it = o_clts.begin(); it != o_clts.end(); it++)
        uniqueMembers.insert(*it);
    for (std::vector<int>::iterator it = clts.begin(); it != clts.end(); it++)
        uniqueMembers.insert(*it);
    
    for (std::set<int>::iterator it = uniqueMembers.begin(); it != uniqueMembers.end(); it++) {
        if (*it != 0)
            SendMsg(*it, msg);
    }
}

void DataControler::SendMsg(const std::string &channelname, int clientid, std::string msg) {
    Channels *ch = DataControler::getChannel(DataControler::transformCase(channelname));
    std::vector<int> clts = ch->getList(0);
    for (std::vector<int>::iterator it = clts.begin(); it != clts.end(); it++) {
        if (*it != clientid)
            SendMsg(*it, msg);
    }
    clts.clear();
    clts = ch->getList(1);
    for (std::vector<int>::iterator it = clts.begin(); it != clts.end(); it++) {
        if (*it != clientid)
            SendMsg(*it, msg);
    }
}

// void DataControler::RemovePendingInvite(int clientID){

// }
