#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <map>
#include <exception>
#include "CClient.hpp"

class Client;
class Channel {
    
    private:
		int id;
		bool _invite;
		bool _topic;
		bool _key;
        int user_limits;
        std::string creator_name;
        std::string creation_time;
        std::string topic;
        std::string key;
        std::map<std::string, Client> members;
        std::map<std::string, Client> operators;
        std::map<std::string, Client> ban_list;
    
    public:
        Channel();
        ~Channel();
        
		//----------getters-----------
        std::string getCreator(void) const;
        std::string getCreation_Time(void) const;
        std::string getTopic(void) const;
        std::string getKey(void) const;
        int getUserLimits(void) const;
        std::map<std::string, Client> getmembers(void) const;
        std::map<std::string, Client> getoperators(void) const;
        std::map<std::string, Client> getban_list(void) const;
        
		//----------setters-----------
        void setCreator(const std::string& _creator);
        void setCreation_time(const std::string& _time);
        void setTopic(const std::string& _topic);
        void setKey(const std::string& _key);
        void addMember(const std::string& name, const Client& client);
        void removeMember(const std::string& name);
		void SetChannelId(int id);
		//-----------MODE-SETTERS--------
		void SetInviteMode(bool _invite);
		void SettopicMode(bool _topic);
		void SetPassMode(bool _key);
		void SetUserLimits(int _limits);



};

/*
 *	@brief : this class will be used with the server this class will take care of all the client and commands and also channels
 *	@details : start from begining of server will take each client and add it to
 *  @var: ClientIds is a multimap that will hold {client_name, client_class, client_id}
 *  
*/

// class Tools{
	
// 	private:
// 		static std::map<int,Client>ClientIDs;
// 		static std::map<int,Channel>ChannelsList;
// 	public:
// 		// ~Tools();
// 		// Tools();
// 		//----------setters-----------
// 		static void addClient(const int id);
// 		static void addChannel(const int id);
// 		static void initTools(void);
// 		static void removeClient(int id);
// 		static void removeChannel(int id);
// 		static Client &getClientById(int id);
// 		static Channel &getChannelById(int id);
// 		static int DataParse(int id, std::string Message);
// 		static int getKey(std::string Nick);
// 		static int getKey(std::string ChannelName);
// 		//----------getters-----------

// 		//----------Exceptions--------------
// 		// class NoUser : public std::exception {
// 		// 	public:
// 		// 		char * what() const throw();
// 		// };
// };

/*
	Tools::addClient()
 * 
*/


#endif