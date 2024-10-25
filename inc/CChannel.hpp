#ifndef CCHANNEL_HPP
#define CCHANNEL_HPP

#include <map>
#include <exception>
#include "CClient.hpp"

class Client;

class Channel {
    private:
        int id; // each channel will have a unique id 
        int limits;
        int mode; //(idea 2)
        /*
            because we have just 4 Flags we can use 4 bits to store them or we can use int to store them

            (idea 1 using 4 bits bitmasks to store and check the flags) (& check | set &~ clear).
            0000 = if all flags are false and limits not set (-1)
            0001 = if inviteonly flag is true
            0010 = if topic flag is true
            0011 = if inviteonly flag is true and topic flag is true
            0100 = if password key flag is true
            0101 = if password key flag is true and inviteonly flag is true
            0110 = if password key flag is true and topic flag is true
            0111 = if password key flag is true and topic flag is true and inviteonly flag is true
            1000 = if user limits is > 0
            1001 = if user limits is > 0 and inviteonly flag is true
            1010 = if user limits is > 0 and topic flag is true
            1011 = if user limits is > 0 and topic flag is true and inviteonly flag is true
            1100 = if user limits is > 0 and password key flag is true
            1101 = if user limits is > 0 and password key flag is true and inviteonly flag is true
            1110 = if user limits is > 0 and password key flag is true and topic flag is true
            1111 = if all flags are true and user limits is > 0

            (idea 2 using int + to store (%, /) to check the flags) (+ set %,/ check - clear).
            0000 : if all flags are false and limits not set (-1)
            1111 : if all flags are true and user limits is > 0
        */
        std::string creatorName;
        std::string creationTime;
        std::string topic;
        std::string key;
        std::map<std::string, Client> members;
        std::map<std::string, Client> operators;
        std::map<std::string, Client> banList;
    public:
        Channel();
        ~Channel();
        //----------Global-Getters-----------
        int getChannelId() const;
        std::string getCreator() const;
        std::string getCreationTime() const;
        std::string getTopic() const;
        std::string getKey() const;
        std::map<std::string, Client> getMembers() const;
        std::map<std::string, Client> getOperators() const;
        std::map<std::string, Client> getBanList() const;
        //----------Global-Setters-----------
        void setCreator(const std::string& creator);
        void setCreationTime(const std::string& time);
        void setTopic(const std::string& topic);
        void setKey(const std::string& key);
        void addMember(const std::string& name, const Client& client);
        void removeMember(const std::string& name);
        void setChannelId(int id);
        //-----------MODE-SETTERS--------
        void setMode(int mode);
        void setUserLimits(int _limits);
        //-----------MODE-GETTERS--------
        int getMode() const;
        int getUserLimits() const;
};

/*
 *  @brief : this class will be used with the server this class will take care of all the client and commands and also channels
 *  @details : start from beginning of server will take each client and add it to
 *  @var: ClientIds is a multimap that will hold {client_name, client_class, client_id}
 *  
*/

// class Tools{
    
//  private:
//      static std::map<int,Client>ClientIDs;
//      static std::map<int,Channel>ChannelsList;
//  public:
//      // ~Tools();
//      // Tools();
//      //----------setters-----------
//      static void addClient(const int id);
//      static void addChannel(const int id);
//      static void initTools(void);
//      static void removeClient(int id);
//      static void removeChannel(int id);
//      static Client &getClientById(int id);
//      static Channel &getChannelById(int id);
//      static int DataParse(int id, std::string Message);
//      static int getKey(std::string Nick);
//      static int getKey(std::string ChannelName);
//      //----------getters-----------

//      //----------Exceptions--------------
//      // class NoUser : public std::exception {
//      //  public:
//      //      char * what() const throw();
//      // };
// };

/*
    Tools::addClient()
 * 
*/

#endif // CCHANNEL_HPP