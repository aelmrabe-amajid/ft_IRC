#include "CChannel.hpp"


// ---------------Channel---------------

Channel::Channel() {}

Channel::~Channel() {}

std::string Channel::getCreator(void) const {
    return this->creator_name;
}

std::string Channel::getCreation_Time(void) const {
    return this->creation_time;
}

std::string Channel::getTopic(void) const {
    return this->topic;
}

std::string Channel::getKey(void) const {
    return this->key;
}

int Channel::getUserLimits(void) const {
    return this->user_limits;
}

std::map<std::string, Client> Channel::getmembers(void) const {
    return this->members;
}

std::map<std::string, Client> Channel::getoperators(void) const {
    return this->operators;
}

std::map<std::string, Client> Channel::getban_list(void) const {
    return this->ban_list;
}

void Channel::setCreator(const std::string& _creator) {
    this->creator_name = _creator;
}

void Channel::setCreation_time(const std::string& _time) {
    this->creation_time = _time;
}

void Channel::setTopic(const std::string& _topic) {
    this->topic = _topic;
}

void Channel::setKey(const std::string& _key) {
    this->key = _key;
}

void Channel::addMember(const std::string& name, const Client& client) {
    this->members[name] = client;
}

void Channel::removeMember(const std::string& name) {
    this->members.erase(name);
}

void Channel::SetChannelId(int id){
	this->id = id;
}

void Channel::SetInviteMode(bool _invite){
	this->_invite = _invite;
}

void Channel::SetPassMode(bool _key){
	this->_key = _key;
}

void Channel::SetUserLimits(int _limits){
	this->user_limits = _limits;
}

void Channel::SettopicMode(bool _topic){
	this->_topic = _topic;
}

// ---------------Commands---------------


// ---------------Tools---------------

// void Tools::addClient(const int id)
// {
// 	Client user;

// 	user.setuserId(id);
// 	user.setuserName("");
// 	user.setrealName("");
// 	user.setnickName("");
// 	user.setState(true);
// 	ClientIDs[id] = user;
// }

// Client& Tools::getClientById(int id)
// {
// 	std::map<int,Client>::iterator it = ClientIDs.find(id);
// 	if(it != ClientIDs.end())
// 		return (it->second);
// 	else
// 		throw(0);
// }

// /*
// 	@brief : this function take NickName iterate in the map
// 	if there is any element in the map have this NickName
// 	we will return the key of this element
// 	@return : key of element in map | -1 if not found
// */
// int Tools::getKey(std::string Nick)
// {
// 	std::map<int,Client>::iterator it = ClientIDs.begin();
// 	while (it != ClientIDs.end())
// 	{
// 		if (it->second.getnickName() == Nick)
// 			return (it->first);
// 		it++;
// 	}
// 	return (-1);
// }

// void Tools::initTools(void)
// {

// }

// char *Tools::NoUser::what() const{
// 	return ("No User with This ID");
// }

// std::vector<std::string> MsgSplit(std::string Message)
// {
// 	std::vector<std::string> res;
// 	size_t beggin = 0;
// 	size_t end = Message.find(" ");
// 	while(end != std::string::npos)
// 	{
// 		res.push_back(Message.substr(beggin, end - beggin));
// 		while(Message[end] == ' ')
// 			end++;
// 		beggin = end;
// 		end = Message.find(" ", beggin);
// 	}
// 	return res;
// }

// /*
// 	this function following this Format
// 	id : the client ID (is file Discriptor of the current client)
	
// 	Message : {Command, TextContent}
// 		-> Command : In the case of Command The av[1] must be a valid Command
// 			->Tookenis("Message");
// 		-> TextContent : Any Thing else Not command.
	
// */
// int Tools::DataParse(int id, std::string Message)
// {
// 	std::vector<std::string> Tookens = MsgSplit(Message);
// 	if (Tookens.size() > 0)
// 	{
// 		if (Tookens.at(0) == )
// 	}

// }