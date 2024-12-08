#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "DataControler.hpp"
#include <exception>
#include <map>

enum CommandID {
    PASS,
    NICK,
    USER,
    JOIN,
    MODE,
    QUIT,
    PART,
	INVITE,
	KICK,
    TOPIC,
    PRIVMSG,
	PONG,
    UNKNOWN
};

class Command {
	public:
	    virtual ~Command() {}
	    Command(int _clientID) : clientID(_clientID) {};
	    virtual void execute() = 0;
	    static Command *createCommand(int clientID, const std::string& params, CommandID command);
		static void HandleCommand(int clientID, const std::string& message);
		static void HandleCommand(int clientID, std::vector<std::string>& message);
		static std::string transformCase(std::string &str);
	protected:
    	int clientID;
    	CommandID command;
};

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
		std::string psw;
};

class UnknownCommand : public Command {
	public:
		UnknownCommand();
	    UnknownCommand(int _clientID, const std::string& _message);
	    void execute();
		~UnknownCommand();
	private:
		std::string message;
};

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


class JoinCommand : public Command {
	public:
		JoinCommand();
		~JoinCommand();
	    JoinCommand(int _clientID, const std::string& _message);
		void joinExistingChannel(const std::string& channel, const std::string& key);
		void joinNewChannel(const std::string& channel);
	    void execute();
	private:
    	std::string message;
		std::vector<std::string> channels;
		std::vector<std::string> keys;
};

class ModeCommand : public Command {
	public:
		ModeCommand();
		~ModeCommand();
	    ModeCommand(int _clientID, const std::string& _message);
		void fillModeInfos();
	    void execute();
	private:
		std::string message;
		std::string	target;
		std::string	mode;
		std::vector<std::string> params;
};

class QuitCommand : public Command {
	public:
		QuitCommand();
		~QuitCommand();
		QuitCommand(int _clientID, const std::string& _message);
		void execute();
	private:
		std::string message;
		std::string params;
};

class PartCommand : public Command {
	public:
		PartCommand();
		~PartCommand();
		PartCommand(int _clientID, const std::string& _message);
		void execute();
	private:
		std::string message;
		std::vector<std::string> channels;
		std::string reason;
};

class InviteCommand : public Command {
	public:
		InviteCommand();
		~InviteCommand();
		InviteCommand(int _clientID, const std::string& _message);
		void execute();
	private:
		std::string message;
		std::string nickname;
		std::string channel;
};

class KickCommand : public Command {
	public:
		KickCommand();
		~KickCommand();
		KickCommand(int _clientID, const std::string& _message);
		void execute();
	private:
		std::string message;
		std::string channel;
		std::string nickname;
		std::string reason;
};


class TopicCommand : public Command {
	public:
		TopicCommand();
		~TopicCommand();
		TopicCommand(int _clientID, const std::string& _message);
		void execute();
	private:
		std::string message;
		std::string channel;
		std::string topic;
};


class PrivmsgCommand : public Command {
	public:
		PrivmsgCommand();
		~PrivmsgCommand();
		PrivmsgCommand(int _clientID, const std::string& _message);
		void execute();
	private:
		std::string message;
		std::vector<std::string> target;
		std::string msg;
};

#endif // COMMANDS_HPP