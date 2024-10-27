#include "../../inc/CClient.hpp"

Client::Client(void){}

Client::~Client(void){}

Client &Client::operator=(const Client& obj)
{
	if (this == &obj)
		return(*this);
	this->id = obj.id;
	this->IPadd = obj.IPadd;
	this->data = obj.data;
	this->message = obj.message;
	this->state = obj.state;
	this->userName = obj.userName;
	this->nickName = obj.nickName;
	this->realName = obj.realName;
	return (*this);
}

bool Client::getstate(void) const {
	return this->state;
};

std::string Client::getuserName(void) const {
	return (this->userName);
}

std::string Client::getnickName(void) const {
	return (this->nickName);
}

std::string Client::getrealName(void) const {
	return (this->realName);
}

int Client::getuserId(void) const{
	return (this->id);
}

void Client::setState(bool _state) {
    this->state = _state;
}


void Client::setuserId(int id)
{
	this->id = id;
}

void Client::setuserName(const std::string& _userName) {
    this->userName = _userName;
}

void Client::setnickName(const std::string& _nickName) {
    this->nickName = _nickName;
}

void Client::setrealName(const std::string& _realName) {
    this->realName = _realName;
}

std::vector<int> Client::getChannelsList(void)const {
	return (this->channelList);
}

void Client::addChannelid(int channelId){
	this->channelList.push_back(channelId);	
}

void Client::removeChannelid(int channelId){
	for (std::vector<int>::iterator it = this->channelList.begin(); it != this->channelList.end(); it++)
		if (*it == channelId)
			this->channelList.erase(it);
}