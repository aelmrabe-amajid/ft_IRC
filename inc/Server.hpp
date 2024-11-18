#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP


// class Server{

// 	private:

// 		int Port;
// 		int SerSocketFd;
// 		static bool Signal;
// 		std::vector<struct pollfd> fds;
// 		std::string Passwd;

// 	public:
// 		// CONSTRUCTOR AND DESTRUCTOR
// 		Server(){SerSocketFd = -1;}
// 		~Server();

// 		// SERVER LANUCH AND KILL
// 		void ServerInit(int port, std::string passwd); //-> server initialization
// 		void SerSocket(); //-> server socket creation
// 		void ServerKill(); //-> server kill

// 		// CLIENT CONNECT AND DISCONNECT
// 		void ClientConnect();
// 		void ClientDisconnect(int fd);

// 		// MESSAGE SENDING AND RECEIVING
// 		void SendToClient(int fd, std::string data);
// 		void SendToAll(std::string data);

// 		// CLIENT INTERACTION
// 		void CommandHandler(std::string command, int fd);

// };


#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
//-------------------------------------------------------//
#define RED "\e[1;31m" 
#define WHI "\e[0;37m" 
#define GRE "\e[1;32m" 
#define YEL "\e[1;33m" 
//-------------------------------------------------------//
#include "./DataControler.hpp"

// #include "./channels/channels.hpp"
// class Client //-> class for client
// {
// private:
//     int Fd; //-> client file descriptor
//     std::string IPadd; //-> client ip address
//     std::string data; // recived string from the client
//     std::string message; //-> message received from the client
// public:
//     Client(){}; //-> default constructor
//     int GetFd(){return Fd;} //-> getter for fd

//     void SetFd(int fd){Fd = fd;} //-> setter for fd
//     void setIpAdd(std::string ipadd){IPadd = ipadd;} //-> setter for ipadd
//     void setMessage(std::string msg){message = msg;} //-> setter for message
//     std::string getMessage(){return message;} //-> getter for message
// };


/*
	Class Server will Be The General Class will Include Multiple Channels Class Instances Also Clients Class Instances
	All The Commands or Operations Will Be Encapsulated in Commands Class. so 3 Classes Will Be Used inside Server Class
*/
class Server //-> class for server
{
	private:
		int Port; //-> server port
		int SerSocketFd; //-> server socket file descriptor
		static bool Signal; //-> static boolean for signal

		// std::vector<Client> clients; //-> vector of clients
		
		std::vector<struct pollfd> fds; //-> vector of pollfd
		std::string Passwd; //-> server password
	public:
		Server(){SerSocketFd = -1;} //-> default constructor
		void ServerInit(int port, std::string passwd); //-> server initialization
		void SerSocket(); //-> server socket creation
		void AcceptNewClient(); //-> accept new client
		void ReceiveNewData(int fd); //-> receive new data from a registered client

		// std::vector<Client> &GetClients(){return clients;} //-> getter for clients
		
		static void SignalHandler(int signum); //-> signal handler
		void CloseFds(); //-> close file descriptors
		void ClearClients(int fd); //-> clear clients
		void SendData(int fd, std::string data); //-> send data to a registered client
		void SendAll(std::string data); //-> send data to all clients
};


#endif // SERVER_HPP