#include "../inc/DataControler.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <set>

std::map<int, Clients> DataControler::clientslist;
std::map<std::string, Channels> DataControler::channelslist;
std::map<std::string, int> DataControler::nicknames;
std::string DataControler::password;

void DataControler::clearData() {
    for (std::map<int, Clients>::iterator it = clientslist.begin(); it != clientslist.end(); ++it)
        close(it->first);
    channelslist.clear();
    nicknames.clear();
}

void DataControler::initData(std::string password) {
    std::string WelcomeMessage = "Welcome to the IRC Server\n";
    std::cout << WelcomeMessage;
    DataControler::password = password;
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

/*
    channelnames are unique and case-insensitive so we will use std::find
*/
bool DataControler::channelnamesExist(const std::string& channelname) {
    std::string ch_lower = transformCase(channelname);
    if (channelslist.find(ch_lower) != channelslist.end())
        return true; 
    return false;
}


void DataControler::addClient(int fd) {
    Clients client = Clients(fd);
    clientslist[fd] = client;
}

void DataControler::addNicknames(const std::string& nickname, int clientID) {
    std::string nickname_lower = transformCase(nickname);
    nicknames[nickname_lower] = clientID;
}

void DataControler::modifyClientNickname(int fd, const std::string& newNickname)
{
    std::string nickname_lower = transformCase(newNickname);
    if (clientslist.find(fd) != clientslist.end())
        nickname_lower = clientslist[fd].getNickName();
    nicknames.erase(nickname_lower);
    nickname_lower = transformCase(newNickname);
    nicknames[nickname_lower] = fd;
}

void DataControler::removeClient(int fd) {
    if (clientslist.find(fd) != clientslist.end()){
        std::cout << "Client <" << fd << "> Disconnected" << std::endl;
        // std::string nickname_lower = clientslist[fd].getNickName();
        // nicknames.erase(nickname_lower);
        clientslist.erase(fd);
        close(fd);
    }
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

bool DataControler::PasswordCheck(const std::string& password) {
    return DataControler::password == password;
}

void DataControler::SendClientMessage(int socketfd, const std::string &message) {
    send(socketfd, message.c_str(), message.size(), 0);
}

//--------------------------------------
/*
    extract Tokens from the message depending on the command and number of parameters possible
    * JOIN <channelname>{,<channelname>} [<key>{,<key>}]
        means that params will be "<channelname>{,<channelname>} [<key>{,<key>}]" and we will save reset of the message in extra

    trailing        ::=  *( ":" / " " / nospcrlfcl )
*/
// std::vector<std::string> DataParser::customSplit(const std::string& str, int pMax, std::string& remaining, std::string& trailing) {
//     std::vector<std::string> parts;
//     std::istringstream stream(str);
//     std::string part;
//     int count = 0;
//     size_t pos = 0;
//     size_t prev_pos = 0;

//     if (str.empty()) return parts;

//     while (count < pMax && (pos = str.find(' ', prev_pos)) != std::string::npos) {
//         part = str.substr(prev_pos, pos - prev_pos);
//         if (!part.empty()) {
//             parts.push_back(part);
//             count++;
//         }
//         prev_pos = pos + 1;
//     }

//     if (count < pMax && prev_pos < str.size()) {
//         part = str.substr(prev_pos);
//         parts.push_back(part);
//         count++;
//     }

//     if (prev_pos < str.size()) {
//         if (str[prev_pos] == ':') {
//             trailing = str.substr(prev_pos + 1); // Skip the colon
//         } else {
//             remaining = str.substr(prev_pos);
//         }
//     } else {
//         trailing.clear();
//         remaining.clear();
//     }

//     return parts;
// }

/*
 *	KICK 	<channel>{,<channel>} <nickname>{,<nickname>} [:<message>] : 3
 *
 *	JOIN 	<channel>{,<channel>} <ch_key>,{<ch_key>} : 2
 *	
 *	PART 	<channel>{,<channel>} [:<message> ] : 2
 *	PRIVMSG <nickname>{,<nickname>} [:<message>] : 2
 *	
 *	MODE 	<channel> <mode>
 *	MODE 	<nickname> <mode>
 *
 *	TOPIC 	<channel> [:<topic>] : 2
 *
 *	NICK 	<nickname> : 1
 *	PASS 	<password> : 1
 *
 *	QUIT 	[ :<quit_message> ] : 1
 *	
 *	USER 	<username> 0 * <realname> : 4

*/
// enum CommandID {
//     JOIN,
//     PART,
//     PRIVMSG,
//     MODE,
//     TOPIC,
//     NICK,
//     PASS,
//     QUIT,
//     USER,
//     UNKNOWN
// };

// bool DataParser::commandId(const std::string& command) {
//     if (command == "JOIN") return JOIN;
//     if (command == "PART") return PART;
//     if (command == "PRIVMSG") return PRIVMSG;
//     if (command == "MODE") return MODE;
//     if (command == "TOPIC") return TOPIC;
//     if (command == "NICK") return NICK;
//     if (command == "PASS") return PASS;
//     if (command == "QUIT") return QUIT;
//     if (command == "USER") return USER;
//     return UNKNOWN;
// }

// void DataParser::getPrefix(const std::string& prefix, int getMode) {
//     if (getMode == 1)
//     {
//         msg.prefix.nickname = DataControler::getClient(clientID).getNickName();
//         msg.prefix.username = DataControler::getClient(clientID).getUserName();
//         msg.prefix.hostname = HOSTNAME;
//     }
//     if (getMode == 2)
//     {
//         std::string trimmedPrefix = prefix.substr(1);
//         std::vector<std::string> parts = split(trimmedPrefix, '!');
//         if (parts.size() == 2) {
//             msg.prefix.nickname = parts[0];
//             std::vector<std::string> userHost = split(parts[1], '@');
//             if (userHost.size() == 2) {
//                 msg.prefix.username = userHost[0];
//                 msg.prefix.hostname = userHost[1];
//             }
//         } else {
//             std::vector<std::string> userHost = split(trimmedPrefix, '@');
//             if (userHost.size() == 2) {
//                 msg.prefix.nickname = userHost[0];
//                 msg.prefix.hostname = userHost[1];
//             }
//         }
//     }
// }

// bool DataParser::getCommandProperties(int& maxp, int& minp)
// {
//     msg.command = DataParser::commandId();
// }

// void DataParser::clearMessage() {
//     msg.prefix.clear();
//     msg.command.clear();
//     msg.params.clear();
//     msg.extra.clear();
//     msg.trailing.clear();
// }



// bool DataParser::parseMessage(const int _clientID, const std::string& message)
// {
//     MessageBNF new_msg;
//     std::istringstream stream(message);
//     std::string token;
//     std::string prefix;
//     std::string parm;

//     msg = new_msg;
//     clientID = _clientID;
//     size_t pos = 0;
//     int maxp = 0;
//     int minp = 0;

//     if (message[0] == ':') {
//         pos = message.find(' ');
//         prefix = message.substr(0, pos);
//         pos++;
//     }
//     DataParser::getPrefix(prefix,1);


//     while (pos < message.size() && message[pos] == ' ') {
//         pos++;
//     }
//     size_t command_end = message.find(' ', pos);
//     token = message.substr(pos, command_end - pos);
//     if (!DataParser::commandId(token)) {
//         DataParser::clearMessage();
//         return false;
//     }
    
//     if (!DataParser::getCommandProperties(msg.command, maxp, minp)){
//         DataParser::clearMessage();
//         return false;
//     }
//     DataParser::getParamNumber(maxp, minp);
//     pos = command_end;
    
//     while (pos < message.size() && message[pos] == ' ')
//         pos++;
//     parm = message.substr(pos);
//     msg.params = DataParser::customSplit(parm, param_expected, msg.extra, msg.trailing);
//     return msg;
// }

// std::string DataParser::trim(const std::string& str) {
//     size_t first = str.find_first_not_of(' ');
//     if (first == std::string::npos) return "";
//     size_t last = str.find_last_not_of(' ');
//     return str.substr(first, last - first + 1);
// }

// std::vector<std::string> DataParser::split(const std::string& str, char delimiter) {
//     std::vector<std::string> tokens;
//     std::string token;
//     std::istringstream tokenStream(str);
//     while (std::getline(tokenStream, token, delimiter)) {
//         tokens.push_back(token);
//     }
//     return tokens;
// }