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

//------------------------

IRCControle::IRCControle(void){};
IRCControle::~IRCControle(void){};

int IRCControle::RunCommand(int fd, std::string Command)
{
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
    this->ClientIds[id] = User;
}

void IRCControle::addChannel(int id)
{
    Channel Chn;

    Chn.setChannelId(id);
    Chn.setMode(0);
    Chn.setUserLimits(0);
}
