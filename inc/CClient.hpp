#ifndef CCLIENT_HPP
#define CCLIENT_HPP

#include <iostream>
#include <string>
#include <vector>


class Client
{
	private:
	    int id; //-> client file descriptor
	    std::string IPadd; //-> client ip address
	    std::string data; // recived string from the client
	    std::string message; //-> message received from the client
		bool state;
		std::string userName;
		std::string nickName;
		std::string realName;
		std::vector<int> channelList;
	public:
	    Client();
		~Client();
		Client&operator=(const Client& obj);
	    int GetFd(){return id;}
	    void SetFd(int fd){id = fd;} //-> setter for fd
	    void setIpAdd(std::string ipadd){IPadd = ipadd;} //-> setter for ipadd
	    void setMessage(std::string msg){message = msg;} //-> setter for message
	    std::string getMessage(){return message;} //-> getter for message
		//----------getters-----------
		bool getstate() const;
		std::string getuserName() const;
		std::string getnickName() const;
		std::string getrealName() const;
		int getuserId(void) const;
		std::vector<int> getChannelsList(void) const;
		// int isChannelMemeber(int channel_id) const;
		//----------setters-----------
		void setState(bool _state);
		void setuserName(const std::string &userName);
		void setnickName(const std::string &nickName);
		void setrealName(const std::string &realName);
		void setuserId(int fd);
		void addChannelid(int id);
		void removeChannelid(int id);
};

#endif