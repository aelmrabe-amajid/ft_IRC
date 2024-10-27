#include "../../inc/irccontrole.hpp"

t_content IRCCommandParser::parse() {
    t_content parsed;
    parsed.command = identifyCommand();
    
    switch (parsed.command) {
        case JOIN: parseJoin(parsed); break;
        case PART: parsePart(parsed); break;
        case PRIVMSG: parsePrivmsg(parsed); break;
        case KICK: parseKick(parsed); break;
        case TOPIC: parseTopic(parsed); break;
        case QUIT: parseQuit(parsed); break;
        case NICK: parseNick(parsed); break;
        case USER: parseUser(parsed); break;
        case PASS: parsePass(parsed); break;
        case INVITE: parseInvite(parsed); break;
        default: std::cerr << "Unknown command." << std::endl;
    }
    return parsed;
}

CommandType IRCCommandParser::identifyCommand()
{
    std::istringstream stream(raw_message);
    std::string cmd;
    stream >> cmd;
    if (cmd == "JOIN") return JOIN;
    if (cmd == "PART") return PART;
    if (cmd == "PRIVMSG") return PRIVMSG;
    if (cmd == "KICK") return KICK;
    if (cmd == "TOPIC") return TOPIC;
    if (cmd == "QUIT") return QUIT;
    if (cmd == "NICK") return NICK;
    if (cmd == "USER") return USER;
    if (cmd == "PASS") return PASS;
    if (cmd == "INVITE") return INVITE;
    return UNKNOWN;
}

void IRCCommandParser::parseJoin(t_content& parsed)
{
    std::istringstream stream(raw_message);
    std::string cmd, channels, keys;
    stream >> cmd >> channels;
    
    size_t pos = channels.find(' ');
    if (pos != std::string::npos) {
        keys = channels.substr(pos + 1);
        channels = channels.substr(0, pos);
    }
    std::vector<std::string> pchannels;
    pchannels = splitString(channels,',');
    for (std::vector<std::string>::iterator it = pchannels.begin(); it != pchannels.end(); it++)
    {
        if (*it != "#")
            std::cout << "should print error Here" << std::endl;
    }
    parsed.channels = splitString(channels, ',');
}

void IRCCommandParser::parsePart(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, channels, reason;
    stream >> cmd >> channels;
    
    std::getline(stream, reason);
    if (!reason.empty()) reason = reason.substr(1);
    parsed.channels = splitString(channels, ',');
    parsed.reason = reason.empty() ? "Leaving" : reason;
}

void IRCCommandParser::parsePrivmsg(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, target, message;
    std::vector<std::string> targets;
	stream >> cmd >> target;
    
    std::getline(stream, message);
    if (!message.empty()) message = message.substr(2);
    targets = splitString(target, ',');
	for (int i = 0; targets[i] != *targets.end(); i++)
	{
		// std::cout << targets[i]<< std::endl;
		if (*targets[i].begin() == '#')
			parsed.channels.push_back(targets[i]);
		else
			parsed.users.push_back(targets[i]);
	}
    // parsed.channels = splitString(target, ',');
    parsed.message = message;
}

void IRCCommandParser::parseKick(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, channel, users, reason;
    stream >> cmd >> channel >> users;
    
    std::getline(stream, reason);
    if (!reason.empty()) reason = reason.substr(1);
    
    parsed.channels.push_back(channel);
    parsed.users = splitString(users, ',');
    parsed.reason = reason.empty() ? "Kicked" : reason;
}

void IRCCommandParser::parseTopic(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, channel, topic;
    stream >> cmd >> channel;
    std::getline(stream, topic);
    parsed.channels.push_back(channel);
    parsed.message = topic.empty() ? "" : topic.substr(1);
}
void IRCCommandParser::parseQuit(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, reason;
    stream >> cmd;
    
    std::getline(stream, reason);
    parsed.reason = reason.empty() ? "Quit" : reason.substr(1);
}
void IRCCommandParser::parseNick(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, nick;
    stream >> cmd >> nick;
    parsed.message = nick;
}
void IRCCommandParser::parseUser(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, username, unused, realname;
    stream >> cmd >> username >> unused >> unused;
    std::getline(stream, realname);
    parsed.message = username + " " + realname.substr(1);
}
void IRCCommandParser::parsePass(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, password;
    stream >> cmd >> password;
    parsed.message = password;
}
void IRCCommandParser::parseInvite(t_content& parsed) {
    std::istringstream stream(raw_message);
    std::string cmd, user, channel;
    stream >> cmd >> user >> channel;
    parsed.users.push_back(user);
    parsed.channels.push_back(channel);
}

std::vector<std::string> IRCCommandParser::splitString(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;
    while (std::getline(stream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}


int IRCCommandVerify::Verify(int clientID, t_content content)
{
    switch (content.command) {
    case JOIN: IRCCommandVerify::VerifyJoin(clientID,content); break;
    case PART: IRCCommandVerify::VerifyPart(clientID,content); break;
    case PRIVMSG: IRCCommandVerify::VerifyPrivmsg(clientID,content); break;
    case KICK: IRCCommandVerify::VerifyKick(clientID,content); break;
    case TOPIC: IRCCommandVerify::VerifyTopic(clientID,content); break;
    case QUIT: IRCCommandVerify::VerifyQuit(clientID,content); break;
    case NICK: IRCCommandVerify::VerifyNick(clientID,content); break;
    case USER: IRCCommandVerify::VerifyUser(clientID,content); break;
    case PASS: IRCCommandVerify::VerifyPass(clientID,content); break;
    case INVITE: IRCCommandVerify::VerifyInvite(clientID,content); break;
    default: std::cerr << "Unknown command." << std::endl;
    }
}

int IRCCommandVerify::VerifyJoin(int clientID,t_content content){
    std::cout << "the Command is Join " << std::endl;
    return (1);
};
int IRCCommandVerify::VerifyPart(int clientID,t_content content){
    return (1);
};
int IRCCommandVerify::VerifyPrivmsg(int clientID,t_content content){
    return (1);
};
int IRCCommandVerify::VerifyKick(int clientID,t_content content){
    return (1);
};
int IRCCommandVerify::VerifyTopic(int clientID,t_content content){
    return (1);
};
int IRCCommandVerify::VerifyQuit(int clientID,t_content content){
    return (1);
};
int IRCCommandVerify::VerifyNick(int clientID,t_content content){
    return (1);
};
int IRCCommandVerify::VerifyUser(int clientID,t_content content){
    return (1);
};
int IRCCommandVerify::VerifyPass(int clientID,t_content content){
    return (1);
};
int IRCCommandVerify::VerifyInvite(int clientID,t_content content){
    return (1);
};

//------------------------

IRCControle::IRCControle(void){
    this->Clients = std::map<int, Client>(); //-> create a map of clients
    this->Channels = std::map<int, Channel>(); //-> create a map of channels
};

IRCControle::~IRCControle(void){};

int IRCControle::RunCommand(int clientID, std::string Command)
{
    std::cout << Command << std::endl;
    IRCCommandParser  Parse(Command);
    t_content Content = Parse.parse();
    if (Content.command == UNKNOWN)
    {
        std::cout << "The command is UNKNOWN" << std::endl;
        return (421);
    }
    IRCCommandVerify::Verify(clientID,Content);
        // ClientERR(Content.command,421);
    
    return (0);
}

void IRCControle::addClient(int id)
{
    Client User;
    User.setnickName("");
    User.setuserName("");
    User.setrealName("");
    User.setuserId(id);
    User.setState(false);
    this->Clients[id] = User;
}

void IRCControle::addChannel(int id)
{
    Channel Chn;

    Chn.setChannelId(id);
    Chn.setMode(0);
    Chn.setUserLimits(0);
}

/*
    this function will be used to delete a specifc channel
    (the channel must be deleted when all members of this channel left)
*/
void IRCControle::removeChannel(int id)
{
    for(std::map<int,Channel>::iterator it = this->Channels.begin(); it != this->Channels.end(); it++)
        if ((*it).first == id)
            this->Channels.erase(it);
}

std::map<int,Channel> IRCControle::getChannelsList(void) const{
    return (this->Channels);
}
std::map<int,Client> IRCControle::getClientsList(void) const{
    return (this->Clients);
}

std::vector<int> IRCControle::getClientsIdslist() const{
    std::vector<int> clientslist;
    if (this->getClientsList().size() > 0)
    {
        for (std::map<int,Client>::iterator it = (this->getClientsList()).begin(); it != (this->getClientsList()).end(); it++)
            clientslist.push_back((*it).first);
        return (clientslist);
    }
    return (clientslist);
}

std::vector<int> IRCControle::getChannelsIdsList() const{
    std::vector<int> channellist;
    if (this->getChannelsList().size() > 0)
    {
        for (std::map<int,Channel>::iterator it = (this->getChannelsList()).begin(); it != (this->getChannelsList()).end(); it++)
            channellist.push_back((*it).first);
        return (channellist);
    }
    return (channellist);
}


int IRCControle::getChannelByName(std::string name) const{
    for (std::map<int,Channel>::iterator it = (this->getChannelsList()).begin(); it != (this->getChannelsList()).end(); it++)
    {
        if ((*it).second.getChannelName() == name && name.length() > 0)
            return ((*it).first);
    }
    return (-1);
}

int IRCControle::getClientByName(std::string name) const{
    for (std::map<int,Client>::iterator it = (this->getClientsList()).begin(); it != (this->getClientsList()).end(); it++)
    {
        if ((*it).second.getnickName() == name && name.length() > 0)
            return ((*it).first);
    }
    return (-1);
}

