#ifndef CCHANNEL_HPP
#define CCHANNEL_HPP

#include <map>
#include <exception>
#include "CClient.hpp"

class Client;

class Channel {
    private:
        int id;
        int limits;
        int mode;
        std::string creatorName;
        std::string creationTime;
        std::string name;
        std::string topic;
        std::string key;
        std::vector<int>members;
        std::vector<int>operators;
        std::vector<int>banlist;
    public:
        Channel();
        ~Channel();
        //----------Global-Getters-----------
        int getChannelId() const;
        std::string getCreator() const;
        std::string getCreationTime() const;
        std::string getTopic() const;
        std::string getKey() const;
        std::string getChannelName() const;
        std::vector<int> getMembers() const;
        std::vector<int> getOperators() const;
        std::vector<int> getBanList()const;
        //----------Is-Getters------------
        bool isMember(int clientId) const;
        bool isOperator(int clientId) const;
        bool isBaned(int clientId) const;
        //----------Global-Setters-----------
        void setCreator(const std::string& creator);
        void setCreationTime(const std::string& time);
        void setTopic(const std::string& topic);
        void setKey(const std::string& channelKey);
        void setChannelId(int channelid);
        void addMember(int clientid);
        void givepermission(int clientid);
        void takepermission(int clientid);
        void removeMember(int clientid);
        void banMember(int clientid);
        void ubanMember(int clientid);
        void setChannelName(std::string _name);
        //-----------MODE-SETTERS--------
        void setMode(int mode);
        void setUserLimits(int _limits);
        //-----------MODE-GETTERS--------
        int getMode() const;
        int getUserLimits() const;
};
#endif // CCHANNEL_HPP