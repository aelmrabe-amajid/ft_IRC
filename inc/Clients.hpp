#ifndef CLIENTS_HPP
#define CLIENTS_HPP

// Public Libraries

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "./Macros.hpp"

class Clients
{
	private:
		std::string nickName;
		std::string userName;
		std::string realName;
		std::string hostName;
		std::string hostAddress;
		int socketfd;
		int isRegistred;
		std::vector<std::string> joinedChannels;
		std::map<std::string, std::vector<int> >RecivedInvites;
	public:
		Clients();
		Clients(int socketfd);
		~Clients();
		std::string getNickName() const;
		std::string getUserName() const;
		std::string getRealName() const;
		std::string getHostName() const;
		int getID() const;
		std::vector<std::string> getJoinedChannels() const;
		bool isClientInChannel(const std::string &channelName) const;
		void setRegistrationStatus(int status);
		void setNickName(const std::string &nickname);
		void setUserName(const std::string &username);
		void setRealName(const std::string &realname);
		void setHostName(const std::string &hostname);
		void setHostAddress(const std::string &hostaddress);
		int  getRegistrationStatus() const;
		void joinChannel(const std::string &channelName);
		void leaveChannel(const std::string &channelName);
		void leaveChannel(void);
		std::map<std::string, std::vector<int> >getRecivedInvites(void);
		void PendingInvite(std::string channel_name, int clientID, int type);
		bool isInvited(const std::string &channelName) const;
		// void addInvite(std::string &channel_name);
};

#endif // CLIENTS_HPP