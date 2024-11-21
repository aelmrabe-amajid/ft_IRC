#ifndef RFC_HPP
#define RFC_HPP

#include <string>
#include <map>
#include <iostream>
#include "DataControler.hpp"

#define SERVERNAME "irc.localhost"
#define USERPREFIX(nick) RFC::UPREF(nick)
#define SERVER_VERSION "1.0"
#define SERVER_CREATION_DATE DataControler::serverCreationDate()

typedef enum err_code{
        ERR_NOSUCHNICK = 401,
        ERR_NOSUCHCHANNEL = 403,
        ERR_CANNOTSENDTOCHAN = 404,
        ERR_TOOMANYCHANNELS = 405,
        ERR_NOORIGIN = 409,
        ERR_NOTEXTTOSEND = 412,
        ERR_INPUTTOOLONG = 417,
        ERR_UNKNOWNCOMMAND = 421,
        ERR_NONICKNAMEGIVEN = 431,
        ERR_ERRONEUSNICKNAME = 432,
        ERR_NICKNAMEINUSE = 433, // 5
        ERR_USERNOTINCHANNEL = 441,
        ERR_NOTONCHANNEL = 442,
        ERR_USERONCHANNEL = 443,
        ERR_NEEDMOREPARAMS = 461,
        ERR_ALREADYREGISTRED = 462,
        ERR_NOPERMFORHOST = 463,
        ERR_PASSWDMISMATCH = 464,
        ERR_CHANNELISFULL = 471,
        ERR_UNKNOWNMODE = 472,
        ERR_INVITEONLYCHAN = 473,
        ERR_BANNEDFROMCHAN = 474,
        ERR_BADCHANNELKEY = 475,
        ERR_BADCHANMASK = 476,
        ERR_NOCHANMODES = 477,
        ERR_BANLISTFULL = 478,
        ERR_NOPRIVILEGES = 481,
        ERR_CHANOPRIVSNEEDED = 482,
}err_code;

class RFC{
    
    public:

        static void INIT();
        static std::string ERR(const std::string &nick, err_code code);
        static std::string ERR(const std::string &nick, const std::string &channelname, err_code code);
        static std::string RPL_ISUPPORT(const std::string &nick);
        static std::string RPL_WELCOME(const std::string &nick);
        static std::string RPL_YOURHOST(const std::string &nick);
        static std::string RPL_CREATED(const std::string &nick);
        static std::string RPL_MYINFO(const std::string &nick);
        static std::string RPL_MODE(const std::string &nick, const std::string &channel, const std::string &mode, int flag);
        static std::string RPL_JOIN(const std::string &nick, const std::string &channel, const std::string& modes, const std::string& clientslist, const std::string& topic);
        static std::string RPL_INVITE(const std::string &nick, const std::string &channel, const std::string &target,int flag);
        static std::string RPL_TOPIC(const std::string &nick, const std::string &channel, int code, const std::string &topic);
        static std::string RPL_KICK(const std::string &nick, const std::string &channel, const std::string &target, const std::string &reason);
    private:
        static std::map<int, std::string> ERRORS;
};

#endif // RFC_HPP