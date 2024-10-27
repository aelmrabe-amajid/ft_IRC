#include "../../inc/CChannel.hpp"

// ---------------Channel---------------

Channel::Channel() {}

Channel::~Channel() {}

std::string Channel::getCreator() const {
    return this->creatorName;
}

std::string Channel::getCreationTime() const {
    return this->creationTime;
}

std::string Channel::getChannelName(void) const{
    return (this->name);
}
std::string Channel::getTopic() const {
    return this->topic;
}

std::string Channel::getKey() const {
    return this->key;
}

int Channel::getChannelId() const {
    return this->id;
}

std::vector<int> Channel::getMembers() const{
    return (this->members);
}
std::vector<int> Channel::getOperators() const{
    return (this->operators);
}
std::vector<int> Channel::getBanList() const{
    return (this->banlist);
}

void Channel::setCreator(const std::string& creator) {
    this->creatorName = creator;
}

void Channel::setCreationTime(const std::string& time) {
    this->creationTime = time;
}

void Channel::setTopic(const std::string& topic) {
    this->topic = topic;
}

void Channel::setKey(const std::string& key) {
    this->key = key;
}

void Channel::setChannelId(int id) {
    this->id = id;
}

void Channel::setMode(int mode) {
    this->mode = mode;
}

int Channel::getMode() const {
    return this->mode;
}

void Channel::setUserLimits(int limits) {
    this->limits = limits;
}

int Channel::getUserLimits() const {
    return this->limits;
}
void Channel::setChannelName(std::string _name){
    this->name = _name;
}

bool Channel::isMember(int clientId) const{
    for(std::vector<int>::iterator it = (this->getMembers()).begin(); it != ((this->getMembers()).end()); it++)
        if (*it == clientId)
            return (true);
    return (false);
}

bool Channel::isOperator(int clientId) const{
    for(std::vector<int>::iterator it = (this->getOperators()).begin(); it != (this->getOperators()).end(); it++)
        if (*it == clientId)
            return (true);
        return (false);
}

bool Channel::isBaned(int clientId) const{
    for(std::vector<int>::iterator it = (this->getBanList()).begin(); it != (this->getBanList()).end(); it++)
        if (*it == clientId)
            return (true);
        return (false);
}

void Channel::addMember(int clientId){
    this->getMembers().push_back(clientId);
}

void Channel::givepermission(int clientId){
    this->getOperators().push_back(clientId);
}

void Channel::takepermission(int clientId){
    for (std::vector<int>::iterator it = (this->getOperators()).begin(); it != (this->getOperators()).end(); it++)
        if (*it == clientId)
            (this->getOperators().erase(it));
}

void Channel::removeMember(int clientId){
    for (std::vector<int>::iterator it = (this->getMembers()).begin(); it != (this->getMembers()).end(); it++)
        if (*it == clientId)
            (this->getMembers().erase(it));
    for (std::vector<int>::iterator it = (this->getOperators()).begin(); it != (this->getOperators()).end(); it++)
        if (*it == clientId)
            (this->getOperators().erase(it));
}

void Channel::banMember(int clientId){
    this->removeMember(clientId);
    (this->getBanList()).push_back(clientId);
}

void Channel::ubanMember(int clientId){
    (this->getBanList()).push_back(clientId);
}

