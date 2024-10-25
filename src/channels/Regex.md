## COMMANDS LIST :

- JOIN
- PART
- PRIVMSG
- KICK
- TOPIC
- QUIT
- NICK
- USER
- PASS
- INVITE
- MODE

## COMMAND SYNTAX :

- JOIN : The JOIN command is used by a user to request to start listening a specific channel.
	- Single Channel without Key : JOIN #channel_name
	- Single Channel with key : JOIN #channel_name key
	- Multiple Channel : JOIN #channel1,#channel2,#channel3 key1,key2,key3

- PART : The PART command is used by a user to request to leave a specific channel.
	- Single Channel : PART #channel_name
	- Multiple Channel : PART #channel1,#channel2,#channel3
	- Single Channel with reason : PART #channel_name reason
	- Multiple Channel with reason : PART #channel1,#channel2,#channel3 reason
(Note: The reason is optional, and if multiple channels are provided, only one reason is allowed, if no reason is provided, the default reason is "Leaving")

- PRVMSG : The PRIVMSG command is used to send private messages between users / channel.
	- Single User : PRIVMSG user_name :message
	- Multiple User : PRIVMSG user1,user2,user3 :message
	- Single Channel : PRIVMSG #channel_name :message
	- Multiple Channel : PRIVMSG #channel1,#channel2,#channel3 :message
	- Combination : PRIVMSG user1,#channel1,user2,#channel2 :message
	
(Note: The message is mandatory, and if multiple users/channels are provided, only one message is allowed)

- KICK : The KICK command is used to request the forced removal of a user from a specific channel.
	- Single User : KICK #channel_name user_name
	- Single User with reason : KICK #channel_name user_name reason
	- Multiple Users : KICK #channel_name user1,user2,user3
	- Multiple Users with reason : KICK #channel_name user1,user2,user3 reason
(Note: The reason is optional, and if multiple users are provided, only one reason is allowed, if no reason is provided, the default reason is "Kicked")

- TOPIC : The TOPIC command is used to request the topic of a specific channel.
	- Single Channel : TOPIC #channel_name topic
(Note: The channel name is mandatory, If topic is not given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or lack of one. If topic is an empty string, the topic for the channel will be cleared.)

- QUIT : The QUIT command is used to request the user to leave the server.
	- QUIT
	- QUIT reason
(Note: The reason is optional, if no reason is provided, the default reason is "Quit").

- NICK : The NICK command is used to request the user to change their nickname.
	- NICK new_nick
(Note: The new nickname is mandatory)

- USER : The USER command is used at the beginning of a connection to specify the username and realname of a new user.
	- USER username * 0 realname

- PASS : The PASS command is used to set a password for the server.
	- PASS password
(Note: The password is mandatory)

- INVITE : The INVITE command is used to invite a user to a channel.
	- INVITE user_name #channel_name


## ABNF RULES LIST : 



