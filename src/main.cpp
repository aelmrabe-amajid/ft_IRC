#include "../inc/Server.hpp"

static void f(void){
	system("leaks -q ircserv");
	system("lsof -c ircserv");
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
		ser.ServerInit(static_cast<int>(std::stoll(args[1])), args[2]); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.CloseFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}