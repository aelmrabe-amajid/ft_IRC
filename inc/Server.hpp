#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP


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

class Server //-> class for server
{
	private:
		int Port; //-> server port
		int SerSocketFd; //-> server socket file descriptor
		static bool Signal; //-> static boolean for signal

		// std::vector<Client> clients; //-> vector of clients
		std::map<int, std::string> inputs;
		std::map<int, std::vector<std::string> >inputsMap;
		std::vector<struct pollfd> fds; //-> vector of pollfd
		std::string Passwd; //-> server password
	public:
		Server(){SerSocketFd = -1;} //-> default constructor
		void ServerInit(int port, std::string passwd); //-> server initialization
		void SerSocket(); //-> server socket creation
		void AcceptNewClient(); //-> accept new client
		void ReceiveNewData(int fd); //-> receive new data from a registered client
		int handleMsg(int fd);

		// std::vector<Client> &GetClients(){return clients;} //-> getter for clients
		
		static void SignalHandler(int signum); //-> signal handler
		void CloseFds(); //-> close file descriptors
		void ClearClients(int fd); //-> clear clients
};


#endif // SERVER_HPP