#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "./Macros.hpp"
#include "./DataControler.hpp"
#include <set>
#include <time.h>
#include <ctime>


typedef struct s_Invites{
    std::vector<int> Inviter;
    std::vector<int> Invited;
}t_Invites;


class Channels {

private:
    std::string ChannelName;
    std::vector<int> Members;
    std::vector<int> Operators;
    std::vector<int> Banned;
    t_Invites Invites;
	// std::map<int,int> InviteList;
    std::map<int,std::vector<int> > InviteList; // Inviter | Invited List
    
    bool Public; 
    bool Secret;
    bool TopicSet;
    bool LimitSet;
    std::string Key;
    std::string Topic;
    std::string TopicTime;
    int TopicSetter;
    int MemberLimit;
    int MemberCount;

public:
    Channels();
    Channels(std::string channelName);
    ~Channels();

    // GET AND SET
    std::string getChannelName() const;
    std::vector<int> getList(int ListType) const; // 0 for Members, 1 for Operators, 2 for Banned
    void setTopicMode(bool mode); // false for everyone, true for operators

    void addClientIn(int Type, int client);
    void removeClientFrom(int Type, int client);
    int getClientType(int client) const; // -1 For NoMember, 0 for Members, 1 for Operators, 2 for Banned
    std::string getMemberList();
    std::string getChannelModes();
    int getMemberLimit() const;
    
	// CHANNEL FLAGS
    bool isPublic() const;
    bool isSecret() const;
    bool isTopicSet() const;
    bool isLimitSet() const;


	// CHANNEL OPERATIONS
	// void inviteClient(int target, int source);
    void setKey(const std::string& key);
    void setTopic(const int clientid, const std::string& topic);
    const std::string getTopic() const;
    void setMemberLimit(int limit);
	void banClient(int clientid);
	void unbanClient(int clientid);
	// void kickClient(int clientid);

    void setChannelMode(char mode, bool set);
    void setChannelMode(char mode, bool set, std::string value);
    // CHANNEL OPERATIONS GETTERS
    bool isBanned(const int clientid) const;
    bool isOperator(const int clientid) const;
    bool isMember(const int clientid) const;
    bool isJoined(const int clientid) const;
	bool isInvited(int clientid);
    bool isFull() const;
    bool isKeyValid(const std::string& key) const;
    std::string namReply() const;
    const std::string getTopicSetter() const;
    const std::string getTopicTime() const;
    int getMemberCount() const;
    // std::vector<int> PendingInvitesFrom(int clientid);
    // void RemovePendingInvitesFrom(int clientid);
    // void RemovePendingInvitesFromMembers();
    // void InviteAccepted(int clientid);
    // void addInvite(int inviter, int invited);
    // std::map<int,std::vector<int> > getInviteMap();


    void AddInvite(int inviter, int invited);
    void RmInvite(int inviter, int invited, int type);
};

#endif // CHANNELS_HPP