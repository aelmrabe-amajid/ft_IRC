// #include "commands.hpp"

// /*
// 	@brief https://modern.ircdocs.horse/#nick-message
// 	@section : Bnf Sysntax {
// 		NickName : ( letter / special ) *8( letter / digit / special / "-" )
		
// 	} 
// */
// int Command::NICK(int id, std::string NickName){
// 	Client user;
// 	try{
// 		user = Tools::getClientById(id);
// 		if (NickName.empty())
// 			return (431);
// 		if (Tools::getKey(NickName)!= -1)
// 			return (433);
// 		if (*NickName.begin() == '#' || *NickName.begin() == '&' || *NickName.begin() == ':')
// 			return (432);
// 		for (std::string::iterator it= NickName.begin(); it != NickName.end(); it++)
// 			if (*it == ' ')
// 				return (432);
// 		if (user.getnickName().length() > 0)
// 			std::cout << user.getnickName() << " change his nickname to " << NickName << std::endl;
// 		user.setnickName(NickName);
// 		return (1);
// 	}catch(int i){
// 		return (i); // Not user
// 	}
// };

// /*
// 	@brief https://modern.ircdocs.horse/#pass-message
// */
// int Command::PASS(int id, std::string PassWord, std::string ServerPass){
// 	if (PassWord != ServerPass)
// 		return (464);
// 	return (1);
// };

// /*
// 	@brief https://modern.ircdocs.horse/#privmsg-message
// 	@var target is a vector of targets{user_name, #channel_name}
// */
// int Command::PRVMSG(int id, std::vector<std::string>target, std::string content){
// 	Client user;
// 	try{
// 		user = Tools::getClientById(id);
// 		if (!user.getnickName().length() > 0)
// 		{
// 			std::cout << "You Must Set NickName To User this Command" << std::endl;
// 			return (-1);
// 		}
// 		for (std::vector<std::string>::iterator it= target.begin(); it != target.end(); it++)
// 		{
// 			std::string x_target = *it;
// 			//there is two cases here (NickName of User) (channel name)
// 			if (*x_target.begin() == '#' && )
// 		}
// 	}
// };
// int Command::JOIN(int id, std::vector<std::string>channel, std::vector<std::string>key){};
// int Command::PART(int id, std::vector<std::string>channel, std::string reason){};
// int Command::TOPIC(int id, std::string channel, std::string topic){};
// int Command::KICK(int id, std::vector<std::string>channel, std::vector<std::string>user, std::string reason){};
// int Command::INVIT(int id, std::string nickname, std::string channel){};
// int Command::QUIT(int id, std::string reason){};
// int Command::USER(int id, std::string UserName, std::string RealName){};