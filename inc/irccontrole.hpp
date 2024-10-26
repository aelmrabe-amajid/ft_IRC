#ifndef IRCCONTROLE_HPP
#define IRCCONTROLE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "CChannel.hpp"

class Client;
class Channel;
enum CommandType {
	    JOIN, PART, PRIVMSG, KICK, TOPIC, QUIT, NICK, USER, PASS, INVITE, UNKNOWN
};

typedef struct s_content{
	CommandType command;
	std::vector<std::string> channels;
	std::vector<std::string> users;
	std::string message;
	std::string reason;
}t_content;

class IRCCommandParser{
	
	public:
	IRCCommandParser(const std::string& input) : raw_message(input) {}
    t_content parse();
    CommandType identifyCommand();
	
	private:
    std::string raw_message;
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    
	void parseJoin(t_content& parsed);
    void parsePart(t_content& parsed);
    void parsePrivmsg(t_content& parsed);
    void parseKick(t_content& parsed);
    void parseTopic(t_content& parsed);
    void parseQuit(t_content& parsed);
    void parseNick(t_content& parsed);
    void parseUser(t_content& parsed);
    void parsePass(t_content& parsed);
    void parseInvite(t_content& parsed);
};

class IRCCommandVerify{
    public:
        int VerifyCommand(int id, t_content Content);
};

class IRCReplay{
    public:

    static void ERR(int client_id, int err_num);
    static void RPL(int client_id, int rpl_num);
};

class IRCControle{
    private:
        std::map<int,Client>ClientIds;
        std::map<int,Channel>ChannelIds;
    public:
        IRCControle();
        ~IRCControle();
        int RunCommand(int fd, std::string Command);
        void addClient(int id);
        void addChannel(int id);
};

#endif 

