#ifndef DATA_CONTROLER_HPP
#define DATA_CONTROLER_HPP

#include <map>
#include <vector>
#include <string>
#include "./Clients.hpp"
#include "./Channels.hpp"
#include "./Commands.hpp"
#include "./Macros.hpp"
class Clients;
class Channels;
class Command;


class DataControler {
    public:
        static void clearData(); // Clear Data
        static void initData(std::string password); // Initialize Data
    // Maps Getters
    static Clients* getClient(int fd); // get Client Class by ID
    static Clients* getClient(const std::string& nickname); // get Client Class by Nickname
    static Channels* getChannel(const std::string& channelname); // get Channel Class
    static std::vector<int>& getChannelsMembers(const std::vector<std::string>& channelsName); // get Channel Members without duplicates
    static bool isClient(int fd); // Check if Client is in the List
    static bool isClient(const std::string& nickname); // Check if Client is in the List
    static bool isChannel(const std::string& channelname); // Check if Channel is in the List
    static bool nicknamesUnique(const std::string& nickname); // Check if Nickname is in the List
    static bool channelnamesExist(const std::string& channelname); // Check if Channel Name is in the List
    static std::string getClientNickname(int fd); // Get Client Nickname
    // Maps Setters
    static void addClient(int fd, const std::string& hostname); // Add Client to the List
    static void addNicknames(const std::string& nickname, int clientID); // Add Nickname to the List
    static void modifyClientNickname(int fd, const std::string& newNickname); // Modify Client Nickname
    static void removeClient(int fd, int flag); // Remove Client from the List
    static Channels *addChannel(const std::string& channelname); // Add Channel to the List
    static void removeChannel(const std::string& channelname); // Remove Channel from the List
    static void SendClientMessage(int socketfd, const std::string &message);
    static std::string transformCase(const std::string& str);
    static bool PasswordCheck(const std::string& password);
    // static std::string UPREF(const std::string& nick);
    // static std::string UPREF(const int clientID);
    static std::string serverCreationDate();
    static void SendMsg(int clientid, std::string msg);
    static void SendMsg(const std::string &channelname, std::string msg);
    static void SendMsg(const std::string &channelname, int clientid, std::string msg);
    // static void RemovePendingInvite(int clientID);
private:
    static std::map<int, Clients> clientslist;
    static std::map<std::string, Channels> channelslist;
    static std::map<std::string, int> nicknames;
    static std::string password;
    static std::string srv_date;
};

// #define UPRF(clientID) DataControler::UPREF(clientID);

#endif // DATA_CONTROLER_HPP