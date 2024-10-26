// #include "../../inc/irccommand.hpp"
// #include <cassert>


// /*
// 	The Only Correct Case : JOIN #channel_name key
	
// 	When The Command Runed without Problem we Must return :
// 		source JOIN channel_name 	
// 	1.  if channel have a Topic :
// 			RPL_TOPIC && RPL_TOPICWHOTIME 
// 		else
// 			nothing

// 	1 : Length of Keys vector more than length of Channels Vectors : (valid)
// 	2 : On of The channels_name don't start with # (ERR_BADCHANMASK)
// 	3 : The Channel Require Key But i dident Provide KeyOfThis Channel (ERR_BADCHANNELKEY )
// 	4 : Empty Join Command without any Parameter (ERR_NEEDMOREPARAMS)
// 	5 : The Channel is invite Only (ERR_INVITEONLYCHAN)
// 	6 : The User has reach the Max Number of Channels that can Join (ERR_TOOMANYCHANNELS)
// 	7 : The User exist in the Ban List of this channel (ERR_BANNEDFROMCHAN)
// 	8 : The Channel not exist So Creating Of Channel But The Channel Name Don't Match Rule Names (ERR_BADCHANMASK)
// 	9 : the Channel Member Limits are reached (ERR_CHANNELISFULL)
// 	Return Of Each Case:

// 	1 : 


// */
// void JoinTest()
// {
// 	std::vector<std::string> inputes;
// 	inputes.push_back("JOIN #my_channel,#my_channel2,#my_channel3 1111,2222,3333,4444"); 
// 	inputes.push_back("JOIN my_channel"); 
// 	inputes.push_back("JOIN #my_channel"); 
// 	inputes.push_back("JOIN"); 
// 	inputes.push_back("JOIN #my_channel4 4444"); 
// 	inputes.push_back("JOIN #my_channel5"); 
// 	inputes.push_back("JOIN #my_channel6"); 
// 	inputes.push_back("JOIN #11my_channel"); 
// 	inputes.push_back("JOIN #my_channel7");

// 	// for(std::vector<std::string>::iterator it = inputes.begin(); it != inputes.end(); it++)
// 	// {

// 	// }
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