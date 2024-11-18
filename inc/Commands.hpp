#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "DataControler.hpp"
#include <map>

typedef struct PrefixBNF {
    std::string nickname;
    std::string username;
    std::string hostname;
}PrefixBNF;

enum CommandID {
    JOIN,
    PART,
    PRIVMSG,
    MODE,
    TOPIC,
    NICK,
    PASS,
    QUIT,
    USER,
    UNKNOWN
};

enum TokensType {
	TY_CHANNEL,
	TY_KEY,
	TY_TARGET,
	TY_MESSAGE,
	TY_MODE,
	TY_TOPIC,
	TY_NICKNAME,
	TY_PASSWORD,
	TY_USERNAME,
	TY_SERVERNAME,
	TY_REALNAME,
	TY_UNKNOWN
};

// Base class for all commands
class Command {
	public:
	    virtual ~Command() {}
	    Command(int _clientID) : clientID(_clientID) {};
	    virtual void execute() = 0;
		static void parse(std::string message);
	    static Command *createCommand(int clientID, const std::string& params, CommandID command);
		void getPrefix(const std::string& prefix, int getMode);
		static void HandleCommand(int clientID, const std::string& message);
		static std::string transformCase(std::string &str);
		static bool basicCheck(int clientID, CommandID command);
		// int tokenLexerChecker(const std::string& token);
	protected:
    	int clientID;
    	PrefixBNF prefix;
    	CommandID command;
};

class JoinCommand : public Command {
	public:
		JoinCommand();
		~JoinCommand();
	    JoinCommand(int _clientID, const std::string& _message);
		void joinExistingChannel(const std::string& channel, const std::string& key);
		void joinNewChannel(const std::string& channel);
	    void execute();
	private:
    	std::string message;// #ch key 
		std::vector<std::string> channels;
		std::vector<std::string> keys;
		// std::string ignored;
		// std::vector<int> ResponseCodes;
};

// class PartCommand : public Command {
// 	public:
// 	    PartCommand(int _clientID, const std::string& _message) : Command(_clientID, _message) {}
// 	    bool parse();
// 	    bool execute();
// 	private:
// 		std::vector<std::string> channels;
// 		std::string trailing;
// 		std::vector<int> ResponseCodes;
// };

// class PrivmsgCommand : public Command {
// 	public:
// 	    PrivmsgCommand(int _clientID, const std::string& _message) : Command(_clientID, _message) {}
// 	    bool parse();
// 	    bool execute();
// 	private:
// 		std::string target;
// 		std::string message;
// 		std::vector<int> ResponseCodes;
// };

// class ModeCommand : public Command {
// 	public:
// 	    ModeCommand(int _clientID, const std::string& _message) : Command(_clientID, _message) {}
// 	    bool parse();
// 	    bool execute();
// 	private:
// 		std::string target;
// 		std::string mode;
// 		std::vector<int> ResponseCodes;
// };

// class TopicCommand : public Command {
// 	public:
// 	    TopicCommand(int _clientID, const std::string& _message) : Command(_clientID, _message) {}
// 	    bool parse();
// 	    bool execute();
// 	private:
// 		std::string channel;
// 		std::string topic;
// 		std::vector<int> ResponseCodes;
// };

class NickCommand : public Command {
	public:
		NickCommand();
	    NickCommand(int _clientID, const std::string& message);
		~NickCommand();
	    void execute();
	private:
		std::string nickname;
};

class PassCommand : public Command {
	public:
		PassCommand();
	    PassCommand(int _clientID, const std::string& message);
	    void execute();
		~PassCommand();
	private:
		std::string password;
		// std::vector<int> ResponseCodes;
};

class UnknownCommand : public Command {
	public:
		UnknownCommand();
	    UnknownCommand(int _clientID, const std::string& _message);
	    void execute();
		~UnknownCommand();
	private:
		std::string message;
		// std::vector<int> ResponseCodes;
};

class QuitCommand : public Command {
	public:
		QuitCommand();
	    QuitCommand(int _clientID, const std::string& _message);
		~QuitCommand();
	    void execute();
	private:
		std::string message;
		// std::vector<int> ResponseCodes;
};

/*
	USER username 0 * :realname
*/
class UserCommand : public Command {
	public:
		UserCommand();
	    UserCommand(int _clientID, const std::string& _message);
		~UserCommand();
	    void execute();
	private:
		std::string message;
		std::string username;
		std::string realname;
};

class ModeCommand : public Command{
	public:
		ModeCommand();
		ModeCommand(int _clientID, const std::string& _message);
		~ModeCommand();
		void execute();
	private:
		std::string message;
		std::string adds;
		std::string removed;
		std::string ModeParm;
};

#endif // COMMANDS_HPP