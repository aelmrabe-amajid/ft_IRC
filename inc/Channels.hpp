#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "./Macros.hpp"
#include "./DataControler.hpp"
#include <time.h>
#include <ctime>

class Channels {

private:
    std::string ChannelName;

    std::vector<int> Members;
    std::vector<int> Operators;
    std::vector<int> Banned;
	std::map<int,int> InviteList; // {Invited, Inviter}|{target, source}

    bool Public; // true for public, false for private
    bool Secret; // false for visible without key, true for key
    bool TopicSet; // false everyone can set topic, true only operators can set topic
    bool LimitSet; // false no limit, true limit is set

    std::string Key;
    std::string Topic;
    int TopicSetter;
    std::string TopicTime;
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
	void inviteClient(int target, int source);
    void setKey(const std::string& key);
    void setTopic(const int clientid, const std::string& topic);
    const std::string getTopic() const;
    void setMemberLimit(int limit);
	void banClient(int clientid);
	void unbanClient(int clientid);
	void kickClient(int clientid);

    void setChannelMode(char mode, bool set);
    void setChannelMode(char mode, bool set, std::string value);
    // CHANNEL OPERATIONS GETTERS
    bool isBanned(const int clientid) const;
    bool isOperator(const int clientid) const;
    bool isMember(const int clientid) const;
    bool isJoined(const int clientid) const;
	bool isInvited(int clientid) const;
    bool isFull() const;
    bool isKeyValid(const std::string& key) const;
    std::string namReply() const;

    const std::string getTopicSetter() const;
    const std::string getTopicTime() const;
    int getMemberCount() const;
    std::vector<int> PendingInvitesFrom(int clientid);
    void RemovePendingInvitesFrom(int clientid);
    void RemovePendingInvitesFromMembers();
    void InviteAccepted(int clientid);
    void addInvite(int invited, int inviter);
};

#endif // CHANNELS_HPP