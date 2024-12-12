#include "../inc/Server.hpp"

int main(int arg_num, char **args)
{
    if(arg_num != 3)
    {
        std::cerr << "Usage: " << args[0] << " <port> <password>" << std::endl;
        return 1;
    }
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler); 
		ser.ServerInit(static_cast<int>(std::stoll(args[1])), args[2]);
	}
	catch(const std::exception& e){
		ser.CloseFds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}