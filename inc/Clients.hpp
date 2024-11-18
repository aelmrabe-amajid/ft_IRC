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
		// Client Data
		std::string nickName;
		std::string userName;
		std::string realName;
		int socketfd;
		int isRegistred;  // 0 for not registred, 1 password verified, 2 nickname set 3 username set
		std::vector<std::string> joinedChannels; // List of channels the client joined
		
	public:
		// CONSTRUCTOR AND DESTRUCTOR
		Clients();
		Clients(int socketfd);
		~Clients();

		// BASIC METHODS
		std::string getNickName() const;
		std::string getUserName() const;
		std::string getRealName() const;
		int getID() const;
		std::vector<std::string> getJoinedChannels() const;
		bool isClientInChannel(const std::string &channelName) const;
		void setRegistrationStatus(int status);

		void setNickName(const std::string &nickname);
		void setUserName(const std::string &username);
		void setRealName(const std::string &realname);
		int getRegistrationStatus() const;
		void joinChannel(const std::string &channelName);
		void leaveChannel(const std::string &channelName);
};

#endif // CLIENTS_HPP