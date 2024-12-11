#ifndef MACROS_HPP
#define MACROS_HPP


// # define user_id(nickname, username) (":" + nickname + "!" + username + "@localhost")
# define CHANNELLEN 32
# define CHANLIMIT 10
# define TOPICLEN 255
# define MAXMODS 10
# define KEYLEN 16
# define MODES 10

# define RPL_WELCOME(_host, user_id, nickname) (":" + _host + " " + "001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(_host, client, version) (":" + _host + " " + "002 " + client + " :Your host is " + _host + " running version " + version + "\r\n")
# define RPL_CREATED(_host, client, datetime) (":" + _host + " " + "003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(_host, client, servername, version, user_modes, chan_modes, chan_param_modes) (":" + _host + " " + "004 " + client + " :" + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
# define RPL_ISUPPORT(_host, client, tokens) (":" + _host + " " + "005 " + client + " :" + tokens " are supported by this server\r\n")
# define ERR_UNKNOWNCOMMAND(_host, client, command) (":" + _host + " " + "421 " + client + " " + command + " :Unknown command\r\n")
# define ERR_NEEDMOREPARAMS(_host, client, command) (":" + _host + " " + "461 " + client + " " + command + " :Not enough parameters.\r\n")
# define ERR_NOSUCHCHANNEL(_host, client, channel) (":" + _host + " " + "403 " + client + " #" + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(_host, client, channel) (":" + _host + " " + "442 " + client + " #" + channel + " :The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(_host, client, nick, channel) (":" + _host + " " + "443 " + client + " " + nick + " #" + channel + " :Is already on channel\r\n")
# define RPL_INVITING(user_id, client, nick, channel) (user_id + " 341 " + client + " " + nick + " #" + channel + "\r\n")
# define RPL_JOIN(user_id, channel) (user_id + " JOIN #" + channel + "\r\n")
# define ERR_TOOMANYCHANNELS(_host, client) (":" + _host + " " + "405 " + client + " :You have joined too many channels\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")
# define ERR_INVITEONLYCHAN(client, channel) ("473 " + client + " #" + channel + " :Cannot join channel (+i)\r\n")
# define ERR_BADCHANMASK(client, channel) ("476 " + client + " #" + channel + " :Bad Channel Mask\r\n")
# define ERR_USERNOTINCHANNEL(client, nickname, channel) ("441 " + client + " " + nickname + " #" + channel + " :They aren't on that channel\r\n")
# define RPL_KICK(user_id, channel, kicked, reason) (user_id + " KICK #" + channel + " " + kicked + " " + reason + "\r\n")
# define ERR_UNKNOWNMODE(_host, client, mode) (":" + _host + " " + "472 " + client + " " + mode + " :is unknown mode char to me\r\n")
# define ERR_USERSDONTMATCH(client) ("502 " + client + " :Cant change mode for other users\r\n")
# define MODE_CHANNELMSG(user_id, channel, mode) (user_id + " MODE #" + channel + " " + mode + "\r\n")
# define RPL_CHANNELMODEIS(_host, client, channel, mode) (":" + _host + " " + "324 " + client + " #" + channel + " " + mode + "\r\n")
# define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + client + " #" + channel + " :Cannot send to channel\r\n")
# define ERR_CHANNELISFULL(client, channel) ("471 " + client + " #" + channel + " :Cannot join channel (+l)\r\n")
# define ERR_CHANOPRIVSNEEDED(_host, client, channel) (":" + _host + " " + "482 " + client + " #" + channel + " :You're not channel operator\r\n")
# define ERR_INVALIDKEY(_host, client, channel) (":" + _host + " " + "525 " + client + " #" + channel + " :Key is not well-formed\r\n")
// # define ERR_INVALIDMODEPARAM(client, channel, mode, password) ("696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")
# define RPL_NAMREPLY(_host, client, symbol, channel, list_of_nicks) (":" + _host + " " +  "353 " + client + " " + symbol + " #" + channel + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(_host, client, channel) (":" + _host + " " + "366 " + client + " #" + channel + " :End of /NAMES list.\r\n")
# define ERR_NONICKNAMEGIVEN(_host, client) (":" + _host + " " + "431 " + client + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(_host, client, nickname) (":" + _host + " " + "432 " + client + " " + nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(_host, client, nickname) (":" + _host + " " + "433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
# define RPL_NICK(_host, oclient, uclient, client) (":" + oclient + "!" + uclient + "@" + _host + " NICK " +  client + "\r\n")
# define RPL_PART(user_id, channel, reason) (user_id + " PART #" + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")
# define ERR_PASSWDMISMATCH(_host, client) (":" + _host + " " + "464 " + client + " :Password incorrect.\r\n")
# define RPL_QUIT(user_id, reason) (user_id + " QUIT :" + reason + "\r\n")
# define RPL_ERROR(user_id, reason) (user_id + " ERROR :" + reason + "\r\n")
# define ERR_NOSUCHNICK(client, target) ("401 " + client + " " + target + " :No such nick/channel\r\n")
# define RPL_PRIVMSG(nick, target, message) (nick + " PRIVMSG " + target + " " + message + "\r\n")
# define RPL_TOPIC(_host, client, channel, topic) (":" + _host + " " + "332 " + client + " #" + channel + " :" + topic + "\r\n")
# define ERR_ALREADYREGISTERED(_host, client) (":" + _host + " " + "462 " + client + " :You may not reregister.\r\n")
# define ERR_CHNAME(_host, client, chan_name) (":" + _host + " " + "479 " + client + " #" + chan_name + " :Illegal channel name.\r\n" )




typedef struct ModeInfo{
    bool set;
    int  type;
    std::string param;
}ModeInfo;

typedef struct s_Invite{
    std::string channel_name;
    int inviter;
    int invited;
}t_Invite;


#endif 