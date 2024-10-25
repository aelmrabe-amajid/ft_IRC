#ifndef COMMANDS_HPP
#define COMMAND_HPP

class Client;
#include "channels.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
/*
	NICK
	PASS
	PRVMSG
	JOIN
	PART
	MODE
	TOPIC
	KICK
	INVITE
	QUIT

	Command Syntax : 
	-> NICK
		Parameters: NickName
	
	-> PASS
		Parameters: secretpasswordhere 
	
	-> PRVMSG
		Parameters: target Content
		Parameters: target,target Content
	
	-> JOIN
		Parameters: channel key
		Parameters: channel,channel key,key
		Parameters: channel,channel,channel key,key
	
	-> PART
		Parameters: channel
		Parameters: channel,reason
		Parameters: channel,channel
		Parameters: channel,channel reason
	
	-> TOPIC
		Parameters: channel topic
	
	-> KICK
		Parameters: channel user
		Parameters: channel user comment
		Parameters: channel,channel,channel user comment
		Parameters: channel user,user,user comment
	
	-> INVITE
		Parameters: nickname channel
	
	-> QUIT
		Parameters: 
		Parameters: reason 
	
	-> USER
		Parameters: UserName 0 * RealName (In Case The RealName or UserName not provided will be replaced by NickName)
*/
class Command{
	public:
		static int NICK(int id,std::string NickName);
		static int PASS(int id,std::string PassWord, std::string ServerPass);
		static int PRVMSG(int id,std::vector<std::string>target, std::string content);
		static int JOIN(int id,std::vector<std::string>channel, std::vector<std::string>key);
		static int PART(int id,std::vector<std::string>channel, std::string reason);
		static int TOPIC(int id,std::string channel, std::string topic);
		static int KICK(int id,std::vector<std::string>channel, std::vector<std::string>user, std::string reason);
		static int INVIT(int id,std::string nickname, std::string channel);
		static int QUIT(int id,std::string reason);
		static int USER(int id,std::string UserName, std::string RealName);
};
#endif