#include "../inc/Server.hpp"

void Server::ClearClients(int fd){ //-> clear the clients
	for(size_t i = 0; i < fds.size(); i++){ //-> remove the client from the pollfd
		if (fds[i].fd == fd)
			{fds.erase(fds.begin() + static_cast<std::vector<struct pollfd>::difference_type>(i)); break;}
	}
	// for(size_t i = 0; i < clients.size(); i++){ //-> remove the client from the vector of clients
	// 	if (clients[i].GetFd() == fd)
	// 		{clients.erase(clients.begin() + i); break;}
	// }

}

bool Server::Signal = false; //-> initialize the static boolean
void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true; //-> set the static boolean to true to stop the server
}

void	Server::CloseFds(){
	DataControler::clearData(); //-> clear the data
	// for(size_t i = 0; i < clients.size(); i++){ //-> close all the clients
	// 	std::cout << RED << "Client <" << clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
	// 	close(clients[i].GetFd());
	// }

	if (SerSocketFd != -1){ //-> close the server socket
		std::cout << RED << "Server <" << SerSocketFd << "> Disconnected" << WHI << std::endl;
		close(SerSocketFd);
	}
}

// std::vector<std::string> MsgSplit(std::string Message)
// {
// 	std::vector<std::string> res;
// 	size_t beggin = 0;
// 	size_t end = Message.find(" ");
// 	while(end != std::string::npos)
// 	{
// 		// std::cout << beggin << " " << end << "\n";
// 		res.push_back(Message.substr(beggin, end - beggin));
// 		// std::cout << "message = " << Message << "\n";
// 		std::cout << res.back() << "\n";
// 		while(Message[end] == ' ')
// 			end++;
// 		// std::cout << end << "\n";
// 		beggin = end;
// 		end = Message.find(' ', beggin);
// 	}
// 	res.push_back(Message.substr(beggin, Message.size()));
// 	return res;
// }

// int main()
// {

// 	/*
// 		IRCCommand::RunCommand(fd,Command)
// 		{
// 			IRCCommandParser parser(command);
// 			res = IRCCommandVerify verify(fd,parser.parse());
// 			if (res.state != VALIDE)
// 				return (IRCReplay::ERR(fd,res));
// 			return (IRCReplay::RPL(fd,res));
// 		}
// 	*/
// 	std::string input = "JOIN #my_channel,#my_channel2,#my_channel3 1111,2222,3333,4444";
// 	IRCCommandParser parser(input);
// 	t_content result = parser.parse();
// 	std::cout << "Command ID: " << result.command << std::endl;
// 	for (std::vector<std::string>::iterator it = result.channels.begin(); it != result.channels.end(); it++)
// 		std::cout << "channels: " << *it << std::endl;
// 	for (std::vector<std::string>::iterator it = result.users.begin(); it != result.users.end(); it++)
// 		std::cout << "users: " << *it << std::endl;
//     // Output parsed data
//     std::cout << "Message: " << result.message << std::endl;
//     std::cout << "Reason: " << result.reason << std::endl;
//     return 0;
// }

static std::vector<std::string> MsgSplit(std::string Message){
	std::vector<std::string> res;
	size_t beggin = 0;
	size_t end = Message.find_first_of("\r\n");
	while(end != std::string::npos)
	{
		res.push_back(Message.substr(beggin, end - beggin));
		beggin = end + 1;
		if (Message[end] == '\r' && beggin < Message.size() && Message[beggin] == '\n')
			beggin++;
		end = Message.find_first_of("\r\n", beggin);
	}
	if (beggin < Message.size())
		res.push_back(Message.substr(beggin, Message.size() - beggin));
	return res;
}

std::map<int, std::vector<std::string> > inputsMap;
void Server::ReceiveNewData(int fd)
{
    char Message[1024]; //-> buffer for the received data
    memset(Message, 0, sizeof(Message)); //-> clear the buffer

    ssize_t bytes = recv(fd, Message, sizeof(Message) - 1 , 0); //-> receive the data

    if(bytes <= 0){ //-> check if the client disconnected
		/*
		 	Tools::removeClient(fd);
		*/
        // std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		DataControler::removeClient(fd,true); //-> remove the client
		inputsMap.erase(fd);
        // ClearClients(fd); //-> clear the client
        // close(fd); //-> close the client socket
    }
    else{ //-> print the received data
        Message[bytes] = '\0';
        std::cout << YEL << "Client <" << fd << "> Data: " << WHI << Message;
        //here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
		// std::cout << buff << std::endl;
        // Find the client and set the message
		/*
			int typeMessage;
			typeMessage = Tool::DataParse(fd,Message)
			if (typeMessage == COMMAND)
			else if (typeMessage == FILE)
			else if (typeMessage == TEXT)
		*/
		// if (DataControler::isClient(fd)){
		// 	Command::HandleCommand(fd, Message);
		if (DataControler::getClient(fd)->getRegistrationStatus() == 1)
		{
			std::vector<std::string> res = MsgSplit(Message);
			for (size_t i = 0; i < res.size(); i++)
				Command::HandleCommand(fd, res[i]);
			res.clear();
		}
		else {
			std::vector<std::string> res = MsgSplit(Message);
			for (size_t i = 0; i < res.size(); i++){
				if (inputsMap[fd].size() < 3)
					inputsMap[fd].push_back(res[i]);
				else
					break;
			}
			if (inputsMap[fd].size() == 3){
				Command::HandleCommand(fd, inputsMap[fd]);
				inputsMap[fd].clear();
			}
			res.clear();
		}
    }
}

void Server::AcceptNewClient()
{
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len = sizeof(cliadd);

	int incofd = accept(SerSocketFd, reinterpret_cast<sockaddr *>(&cliadd), &len); //-> accept the new client
	// int incofd = accept(SerSocketFd, (sockaddr *)&(cliadd), &len); //-> accept the new client
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}

	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		{std::cout << "fcntl() failed" << std::endl; return;}

	NewPoll.fd = incofd; //-> add the client socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0

	/*
		class tools provide a addClient(int fd) // this method will create client class for you 
		and will add client to map ClientIDs. 
		cli = Tools::getClientByID(id)
	*/
	// DataControler::addClient(incofd); //-> add the client to the map of clients
	DataControler::addClient(incofd,inet_ntoa((cliadd.sin_addr))); //-> add the client to the map of new clients
	// cli.SetFd(incofd); //-> set the client file descriptor
	// cli.setIpAdd(inet_ntoa((cliadd.sin_addr))); //-> convert the ip address to string and set it
	// clients.push_back(cli); //-> add the client to the vector of clients
	fds.push_back(NewPoll); //-> add the client socket to the pollfd

	std::cout << incofd << inet_ntoa((cliadd.sin_addr)) << std::endl;
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::SerSocket()
{
	int en = 1;
	struct sockaddr_in add;
	struct pollfd NewPoll;
	add.sin_family = AF_INET; //-> set the address family to ipv4
	add.sin_addr.s_addr = INADDR_ANY; //-> set the address to any local machine address
	add.sin_port = htons(this->Port); //-> convert the port to network byte order (big endian)

	SerSocketFd = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
	std::cout <<"the server socket " <<  SerSocketFd << std::endl;
	if(SerSocketFd == -1) //-> check if the socket is created
		throw(std::runtime_error("faild to create socket"));

	if(setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(SerSocketFd, reinterpret_cast<struct sockaddr *>(&add), sizeof(add)) == -1) //-> bind the socket to the address
	// if (bind(SerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1) //-> bind the socket to the address

		throw(std::runtime_error("faild to bind socket"));
	if (listen(SerSocketFd, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("listen() faild"));

	NewPoll.fd = SerSocketFd; //-> add the server socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0
	fds.push_back(NewPoll); //-> add the server socket to the pollfd
}

void Server::ServerInit(int port, std::string passwd)
{
	this->Port = port; //-> set the server port
	SerSocket(); //-> create the server socket


	DataControler::initData(passwd); //-> initialize the data

	std::cout << GRE << "Server <" << SerSocketFd << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";

	while (Server::Signal == false){ //-> run the server until the signal is received

		if((poll(&fds[0], static_cast<nfds_t>(fds.size()), -1) == -1) && Server::Signal == false) //-> wait for an event
			throw(std::runtime_error("poll() faild"));
		// if((poll(&fds[0],fds.size(),-1) == -1) && Server::Signal == false) //-> wait for an event
		// 	throw(std::runtime_error("poll() faild"));

		for (size_t i = 0; i < fds.size(); i++){ //-> check all file descriptors
			if (fds[i].revents & POLLIN){ //-> check if there is data to read
				if (fds[i].fd == SerSocketFd)
					AcceptNewClient(); //-> accept new client
				else
					ReceiveNewData(fds[i].fd); //-> receive new data from a registered client
			}
		}
	}
	CloseFds(); //-> close the file descriptors when the server stops
}

// void Server::SendData(int fd, std::string data)
// {
//     if (send(fd, data.c_str(), data.size(), 0) == -1) //-> send data to a registered client
//         throw(std::runtime_error("send() faild"));
// }

// void Server::SendAll(std::string data)
// {
//     for(size_t i = 0; i < clients.size(); i++) //-> send data to all clients
//         SendData(clients[i].GetFd(), data);
// }
