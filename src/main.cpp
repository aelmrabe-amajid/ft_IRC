#include "../inc/Server.hpp"

void f()
{
	system("leaks -q ircserv");
}

int main(int arg_num, char **args)
{
	atexit(f);
    if(arg_num != 3) //-> check the number of arguments
    {
        std::cerr << "Usage: " << args[0] << " <port> <password>" << std::endl;
        return 1;
    }
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
		ser.ServerInit(std::stoll(args[1]), args[2]); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.CloseFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}