#include "../inc/RFC.hpp"

std::map<int, std::string> RFC::ERRORS;

void RFC::INIT() {
    ERRORS[401] = ":No such nick/channel";
    ERRORS[403] = ":No such channel";
    ERRORS[404] = ":Cannot send to channel";
    ERRORS[405] = ":You have joined too many channels";
    ERRORS[412] = ":No text to send";
    ERRORS[421] = ":Unknown command";
    ERRORS[431] = ":No nickname given";
    ERRORS[432] = ":Erroneous nickname";
    ERRORS[434] = ":Erroneous username";         
    ERRORS[433] = ":Nickname is already in use";
    ERRORS[441] = ":They aren't on that channel";
    ERRORS[442] = ":You're not on that channel";
    ERRORS[443] = ":is already on channel";
    ERRORS[461] = ":Not enough parameters";
    ERRORS[462] = ":You may not reregister";
    ERRORS[464] = ":Password Incorrect";
    ERRORS[471] = ":Cannot join channel (+l)";
    ERRORS[473] = ":Cannot join channel (+i)";
    ERRORS[474] = ":Cannot join channel (+b)";
    ERRORS[475] = ":Cannot join channel (+k)";
    ERRORS[476] = ":Invalid channel name";
    ERRORS[482] = ":You're not channel operator";
    ERRORS[417] = ":Input line was too long";
    ERRORS[472] = ":is unknown mode char to me";
}


std::string RFC::RPL_ISUPPORT(const std::string &nick) {
    return ":" + std::string(SERVERNAME) + "005 " + nick + " CASEMAPPING=rfc1459 CHANLIMIT=#:50 CHANMODES=k,l,ti CHANNELLEN=50 CHANTYPES=# NICKLEN=9 PREFIX=(o)@+ USERLEN=9 TOPICLEN=390";
}

std::string RFC::RPL_WELCOME(const std::string &nick) {
    return ":" + std::string(SERVERNAME) + "001 " + nick + " :Welcome to the Internet Relay Network " + nick;
}

std::string RFC::RPL_YOURHOST(const std::string &nick) {
    return ":" + std::string(SERVERNAME) + "002 " + nick + " :Your host is " + std::string(SERVERNAME) + ", running version " + std::string(SERVER_VERSION);
}

std::string RFC::RPL_CREATED(const std::string &nick) {
    return ":" + std::string(SERVERNAME) + "003 " + nick + " :This server was created on " + std::string(SERVER_CREATION_DATE);
}

std::string RFC::RPL_MYINFO(const std::string &nick) {
    return ":" + std::string(SERVERNAME) + "004 " + nick + " " + std::string(SERVER_CREATION_DATE) + " ol itkol lok";
}

std::string RFC::ERR(const std::string &nick, err_code code) {
    if (ERRORS[code].empty()) {
        std::cerr << "Unknown ERR : " << code << std::endl;
        return ;
    }
    return ":" + std::string(SERVERNAME) + std::to_string(code) + " " + nick + " " + ERRORS[code].data() + "\r\n";
}

std::string RFC::ERR(const std::string &nick, const std::string &channel, err_code code) {
    if (ERRORS[code].empty()) {
        std::cerr << "Unknown ERR : " << code << std::endl;
        return ;
    }
    // SERVER_PREFIX + errNum + " " + clt.nickname;
    return ":" + std::string(SERVERNAME) + std::to_string(code) + " " + nick + " " + channel + " " + ERRORS[code].data() + "\r\n";
}

std::string RFC::RPL_JOIN(const std::string &nick, const std::string &channel, const std::string& modes, const std::string& clientslist, const std::string& topic){
    std::string str;
    str = ":" + USERPREFIX(nick) + " " + "JOIN" + " " + channel 
    + "\r\n" + RFC::RPL_MODE(nick, channel, modes, 1)
    + "\r\n" + ":" + std::string(SERVERNAME) + "353 " + nick + " @ " + channel + " :" + clientslist 
    + "\r\n" + std::string(SERVERNAME) + "366 " + nick + " " + channel + " :End of /NAMES list."; 
    if (topic.empty() == false)
        str += "\r\n" + RFC::RPL_TOPIC(nick, channel, 0, topic);
    return (str);
}
/*
 commandReply(ch, clt, "MODE", TYPE_SERVER) + " " + ch.getModeString() + "\r\n"
*/
std::string RFC::RPL_MODE(const std::string &nick, const std::string &channel, const std::string &mode, int flag){
    if (flag == 0)
        return (":" + USERPREFIX(nick) + " " + "MODE" + " " + channel + " " + mode);
    else
        return (":" + std::string(SERVERNAME) + " " + "MODE" + " " + channel + " " + mode);
}

std::string RFC::RPL_INVITE(const std::string &nick, const std::string &channel, const std::string &target, int flag){
    if (flag == 0)
        return (":" + USERPREFIX(nick) + " " + "INVITE" + " " + target + " :" + channel);
    else
        return (":" + std::string(SERVERNAME) + " " + "341" + nick + " " + target + " " + channel);
}

std::string RFC::RPL_KICK(const std::string &nick, const std::string &channel, const std::string &target, const std::string &reason){
    return (":" + USERPREFIX(nick) + " " + "KICK" + " " + channel + " " + target + " :" + reason);
}
std::string RFC::RPL_TOPIC(const std::string &nick, const std::string &channel,int code, const std::string &topic) {
    switch (code)
    {
        case 331:
            return (":" + std::string(SERVERNAME) + " " + "331" + " " + nick + " " + channel + " :" + "No topic is set.");
        case 332:
            return (":" + std::string(SERVERNAME) + " " + "332" + " " + nick + " " + channel + " :" + topic);
        case 333:
            return (":" + std::string(SERVERNAME) + " " + "333" + " " + nick + " " + channel + " :" + topic);
        default:
            return (":" + USERPREFIX(nick) + " " + "TOPIC" + " " + channel + " :" + topic);
    }
}
