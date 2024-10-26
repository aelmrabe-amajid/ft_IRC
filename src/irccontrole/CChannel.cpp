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

std::string Channel::getTopic() const {
    return this->topic;
}

std::string Channel::getKey() const {
    return this->key;
}

int Channel::getChannelId() const {
    return this->id;
}

std::map<std::string, Client> Channel::getMembers() const {
    return this->members;
}

std::map<std::string, Client> Channel::getOperators() const {
    return this->operators;
}

std::map<std::string, Client> Channel::getBanList() const {
    return this->banList;
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

void Channel::addMember(const std::string& name, const Client& client) {
    this->members[name] = client;
}

void Channel::removeMember(const std::string& name) {
    this->members.erase(name);
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