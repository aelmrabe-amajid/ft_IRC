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
        static int Verify(int clienID, t_content content);
    
    private:
        static int VerifyJoin(int clientID, t_content content);
        static int VerifyPart(int clientID, t_content content); 
        static int VerifyPrivmsg(int clientID, t_content content); 
        static int VerifyKick(int clientID, t_content content); 
        static int VerifyTopic(int clientID, t_content content); 
        static int VerifyQuit(int clientID, t_content content); 
        static int VerifyNick(int clientID, t_content content); 
        static int VerifyUser(int clientID, t_content content); 
        static int VerifyPass(int clientID, t_content content); 
        static int VerifyInvite(int clientID, t_content content); 
};

class IRCReplay{
    public:
};

class IRCControle{
    private:
        std::map<int,Client>Clients;
        std::map<int,Channel>Channels;
    public:
        //-----Class-Methods-------
        IRCControle();
        ~IRCControle();
        int RunCommand(int fd, std::string Command);     
        //-----Maps-Getters--------
        // std::map<int,Client> getClientIds() const;
        std::map<int,Client>getClientsList()const;
        std::map<int,Channel>getChannelsList()const;
        std::vector<int> getClientsIdslist() const;
        std::vector<int> getChannelsIdsList() const;
        int getChannelByName(std::string name) const;
        int getClientByName(std::string name)const;
        // std::map<int,Channel> getChannelIds() const;
        Channel getChannelById(int id);
        Client getClientById(int id);
        int getFreeChannelId();
        //-----Maps-Setters--------
        void addClient(int id);
        void addChannel(int id);
        void removeClient(int id);
        void removeChannel(int id);
        //------Replays-------------
        void ERR(int client_id, int err_num);
        void RPL(int client_id, int rpl_num);
};

#endif 

