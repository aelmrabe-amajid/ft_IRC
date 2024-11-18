#include "../inc/Commands.hpp"
#include <iostream>
#include <bitset>

static CommandID getCommandId(const std::string& command) {
    if (command == "JOIN") return JOIN;
    if (command == "USER") return USER;
    if (command == "PASS") return PASS;
    if (command == "NICK") return NICK;
    if (command == "QUIT") return QUIT;

    if (command == "MODE") return MODE;
    if (command == "PART") return PART;
    if (command == "PRIVMSG") return PRIVMSG;
    if (command == "TOPIC") return TOPIC;
    return UNKNOWN;
}

// static std::string trim(const std::string& str) {
//     size_t first = str.find_first_not_of(' ');
//     if (first == std::string::npos) return "";
//     size_t last = str.find_last_not_of(' ');
//     return str.substr(first, last - first + 1);
// }

static std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/*
    split by space and take parts number in consideration
    also if part start with : it will be considered as one part at the end of the string
    take in consideration that the part can splited by one or more space
    return vector of strings of the parts of the string
    if parts number is 4 and you find 5 parts it's ok  you will return vector of 5 strings
    if parts number is 4 and you find 1 parts and 2 parts are trailing it's ok you will return vector of 2 strings
    if parts number is 4 and you find 1 parts and 1 parts are trailing it's ok you will return vector of 1 strings
*/
std::vector<std::string> splitParts(const std::string& str, int parts) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(str);
    std::string token;
    int count = 0;

    while (count < parts - 1 && tokenStream >> token) {
        if (token[0] == ':') {
            std::string rest;
            std::getline(tokenStream, rest);
            token += rest;
            tokens.push_back(token);
            return tokens;
        }
        tokens.push_back(token);
        count++;
    }
    if (tokenStream >> token) {
        if (token[0] == ':') {
            std::string rest;
            std::getline(tokenStream, rest);
            token += rest;
        }
        tokens.push_back(token);
    }
    return tokens;
}

void Command::getPrefix(const std::string& prefix, int getMode)
{
    if (getMode == 1)
    {
        this->prefix.nickname = DataControler::getClient(clientID)->getNickName();
        this->prefix.username = DataControler::getClient(clientID)->getUserName();
        this->prefix.hostname = HOSTNAME;
    }
    if (getMode == 2)
    {
        std::string trimmedPrefix = prefix.substr(1);
        std::vector<std::string> parts = split(trimmedPrefix, '!');
        if (parts.size() == 2) {
            this->prefix.nickname = parts[0];
            std::vector<std::string> userHost = split(parts[1], '@');
            if (userHost.size() == 2) {
                this->prefix.username = userHost[0];
                this->prefix.hostname = userHost[1];
            }
        } else {
            std::vector<std::string> userHost = split(trimmedPrefix, '@');
            if (userHost.size() == 2) {
                this->prefix.nickname = userHost[0];
                this->prefix.hostname = userHost[1];
            }
        }
    }
}

// bool DataParser::getCommandProperties(int& maxp, int& minp)
// {
//     msg.command = DataParser::commandId();
// }

// void DataParser::clearMessage() {
//     msg.prefix.clear();
//     msg.command.clear();
//     msg.params.clear();
//     msg.extra.clear();
//     msg.trailing.clear();
// }



// bool DataParser::parseMessage(const int _clientID, const std::string& message)
// {
//     MessageBNF new_msg;
//     std::istringstream stream(message);
//     std::string token;
//     std::string prefix;
//     std::string parm;

//     msg = new_msg;
//     clientID = _clientID;
//     size_t pos = 0;
//     int maxp = 0;
//     int minp = 0;

    // if (message[0] == ':') {
    //     pos = message.find(' ');
    //     prefix = message.substr(0, pos);
    //     pos++;
    // }
    // DataParser::getPrefix(prefix,1);


    // while (pos < message.size() && message[pos] == ' ') {
    //     pos++;
    // }
    // size_t command_end = message.find(' ', pos);
    // token = message.substr(pos, command_end - pos);
    // if (!DataParser::commandId(token)) {
    //     DataParser::clearMessage();
    //     return false;
    // }
    
//     if (!DataParser::getCommandProperties(msg.command, maxp, minp)){
//         DataParser::clearMessage();
//         return false;
//     }
//     DataParser::getParamNumber(maxp, minp);
//     pos = command_end;
    
//     while (pos < message.size() && message[pos] == ' ')
//         pos++;
//     parm = message.substr(pos);
//     msg.params = DataParser::customSplit(parm, param_expected, msg.extra, msg.trailing);
//     return msg;
// }


std::string Command::transformCase(std::string &str) {
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = std::toupper(str[i]);
    }
    return str;
}

bool Command::basicCheck(int clientID, CommandID command) {
    int reg = DataControler::getClient(clientID)->getRegistrationStatus();
    if (reg == 0 && command != PASS && command != NICK)
        return false;
    return true;
}

void Command::parse(std::string message) {
    size_t len = message.length();
    std::cout << "len: " << len << std::endl;
}

void Command::HandleCommand(int clientID, const std::string& _message) {
	// std::istringstream iss(message);
    std::string message = _message;
	std::string prefix = " ";
	std::string command;
	std::string params;
	size_t pos = 0;

    // if (message[0] == ':') {
    //     pos = message.find(' ');
    //     prefix = message.substr(0, pos);
    //     pos++;
    // }
    // remove CR, LF from end of the message
    parse(message);
    if (message[message.size() - 1] == '\n' || message[message.size() - 1] == '\r')
        message.pop_back();
	while (pos < message.size() && message[pos] == ' ') {
        pos++;
    }
    if (message[0] != '/')
        return;
    size_t command_end = message.find(' ', pos);
    if (command_end == std::string::npos) {
        command = message.substr(pos);
        pos = message.size();
    } else {
        command = message.substr(pos, command_end - pos);
        pos = command_end;
    }
    command = command.substr(1);
    command = Command::transformCase(command);
    std::cout << "command: " << command << std::endl;
	while (pos < message.size() && message[pos] == ' ')
		pos++;
	CommandID commandId = getCommandId(command);
    if (message.length() == pos) {
        if (commandId != QUIT)
            return (DataControler::SendClientMessage(clientID, ERR_NEEDMOREPARAMS(" ", command)));
    }
	params = message.substr(pos);
    std::cout << "params: " << params << std::endl;
    std::cout << "commandId: " << commandId << std::endl;
	if (commandId == UNKNOWN) {
		std::cout << "Unknown command" << std::endl;
		return ;
	}
    if (!Command::basicCheck(clientID, commandId)){
        std::cout << "You Must Set NICK" << std::endl;
        return ;
    }
	Command *cmd = Command::createCommand(clientID, params, commandId);
	cmd->getPrefix(prefix, 1);
    cmd->execute();
    // Clients *cl = DataControler::getClient(clientID);
    // if (cmd->command == JOIN && cl->getJoinedChannels().size() > 0)
    //     std::cout << cl->getJoinedChannels().at(0) << std::endl;
	delete cmd;
}


Command *Command::createCommand(int clientID, const std::string& params, CommandID command) {
	Command *cmd;
	switch (command) {
		case JOIN:
			cmd = new JoinCommand(clientID, params);
            break;
        case PASS:
            cmd = new PassCommand(clientID, params);
            break;
        case NICK:
            cmd = new NickCommand(clientID, params);
            break;
        case QUIT:
            cmd = new QuitCommand(clientID, params);
            break;
        case USER:
            cmd = new UserCommand(clientID, params);
            break;
        default:
            cmd = new UnknownCommand(clientID, params);
            break;
	}
	return cmd;
}

//------------------------------------------------------------
/*
* This function will verify the token char and return int based on Char included
* @param token the token to be checked
* @section Char Type BNF
	letter      ::= 'a' ... 'z' | 'A' ... 'Z'
	number      ::= '0' ... '9'
	special     ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
   	white   	::= <SPACE, NUL, CR, LF>
    nochstring  ::= <white> | <BELL and comma (',')>
* @Type Value
 	Alpha 		: 1
 	Number 		: 2
 	Special 	: 4
 	nonwhite 	: 8
 	chstring 	: 16
* @type
	<channel>   ::=  # { 1 | 2 | 3 | 4 | 5 | 6 | 7 }
	<nick> 		::=  1 { 1 | 2 | 3 | 4 | 5 | 6 | 7 }
	<user>      ::=  1 { 1 | 2 | 3 | 4 | 5 | 6 | 7 }
	* @return int value based on char type
* @bug the function will not take in consideration the landing # or ! ... etc
* @note deprecated see(lexicalToken) 
*/
int checkCharsType(std::string token)
{
	int Alpha = 0, Number = 0, Special = 0, nonwhite = 0, chstring = 0;
	for (size_t i = 0; i < token.length(); i++)
	{
		if (isalpha(token[i]))
			Alpha = 1;
		else if (isdigit(token[i]))
			Number = 2;
		else if (token[i] == '-' || token[i] == '[' || token[i] == ']' || token[i] == '\\' || token[i] == '`' || token[i] == '^' || token[i] == '{' || token[i] == '}')
			Special = 4;
		else if (token[i] != ' ' && token[i] != '\0' && token[i] != '\n' && token[i] != '\r')
			nonwhite = 8;
		else if (token[i] != '\a' && token[i] != ',')
			chstring = 16;
	}
	return (Alpha + Number + Special + nonwhite + chstring);
}

// type 1 for channel, 2 nick, 3 user, 4 realname
// user min 1 max 9
// nick min 1 max 9
// channel min 3 max 50
// thinking about regexr ^#[a-zA-Z0-9\-\[\]\\`^{}]{1,50}$
bool checkTokenChar(const std::string& name, int type) {
	int len = name.length();
    if ((type == 1 && !(len >= 4 && len <= 50)) || 
	(type == 2 && !(len >= 1 && len <= 9)) 
	|| (type == 3 && !(len >= 1 && len <= 10)))
        return false;
    if (type == 1 && name[0] != '#')
        return false;
	if ((type == 2 || type == 3) && !isalpha(name[0]))
		return false;
    for (size_t i = 1; i < name.length(); ++i) {
        char c = name[i];
        if (!(
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '-' ||            
            c == '[' ||                
            c == ']' ||                
            c == '\\' ||                
            c == '`' ||                
            c == '^' ||             
            c == '{' ||      
            c == '}'))
        return false;
    }
    return true;
}

//------------------------------------------------------------

JoinCommand::~JoinCommand() {}
JoinCommand::JoinCommand() : Command(0) {
    this->command = JOIN;
}
JoinCommand::JoinCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = JOIN;
    this->message = message;
}

/*
	channel name start with # and contain only letters and numbers or channel name length is less than 50 -> ERR_BADCHANMASK -> 1



	channel exist -> 5
	user already in the channel // ignore without error -> 6
	user banned from the channel // ERR_BANNEDFROMCHAN -> 7
	user reach the max number of channels -> ERR_TOOMANYCHANNELS -> 8
	channel is full -> ERR_CHANNELISFULL -> 9
	channel invite only -> ERR_INVITEONLYCHAN -> 10
	channel key not match -> ERR_BADCHANNELKEY -> 11
	channel join success -> RPL_TOPIC -> 12

	chnamepattern = /#[a-zA-Z0-9][-[]]{1,50}/

*/

/*
    already in the channel -> 1
    banned from the channel -> 2
    channel is full -> 3
    user reach the max number of channels -> 4
    invite only channel -> 5
    bad channel key -> 6
*/
void JoinCommand::joinExistingChannel(const std::string& channel, const std::string& key) {
    std::string chnameL = DataControler::transformCase(channel);
    Channels *ch = DataControler::getChannel(channel);
    std::string channel_name = "#" + ch->getChannelName();
    Clients *cl = DataControler::getClient(clientID);
    
    if (cl->isClientInChannel(chnameL))
        return;
    if (ch->isBanned(clientID))
        return(DataControler::SendClientMessage(clientID,ERR_BANNEDFROMCHAN(this->prefix.nickname, this->prefix.hostname, channel_name)));
    if (ch->isFull())
        return(DataControler::SendClientMessage(clientID,ERR_CHANNELISFULL(this->prefix.nickname, channel_name)));
    if (cl->getJoinedChannels().size() >= CCHANLIMIT)
        return(DataControler::SendClientMessage(clientID,ERR_TOOMANYCHANNELS(this->prefix.nickname, this->prefix.hostname)));
    if (!ch->isPublic() && !ch->isInvited(clientID))
        return(DataControler::SendClientMessage(clientID,ERR_INVITEONLYCHAN(this->prefix.nickname, channel_name)));
    if (ch->isSecret() && !ch->isKeyValid(key))
        return(DataControler::SendClientMessage(clientID,ERR_BADCHANNELKEY(this->prefix.nickname, this->prefix.hostname, channel_name)));
    
    ch->addClientIn(0, clientID);
    cl->joinChannel(chnameL);
    DataControler::SendClientMessage(clientID,RPL_JOIN(this->prefix.nickname, this->prefix.username, channel_name, this->prefix.hostname));
    std::string topic = ch->getTopic();
    if (topic.empty())
        DataControler::SendClientMessage(clientID,RPL_NOTOPIC(this->prefix.hostname, this->prefix.nickname, channel_name));
    else{
        DataControler::SendClientMessage(clientID,RPL_TOPIC(this->prefix.hostname, this->prefix.nickname, channel_name, topic));
        std::string time = ch->getTopicTime();
        std::string setter = ch->getTopicSetter();
        DataControler::SendClientMessage(clientID,RPL_TOPICWHOTIME(this->prefix.hostname, this->prefix.nickname, channel_name, setter, time));
    }
    std::string clients = ch->namReply();
    DataControler::SendClientMessage(clientID,RPL_NAMREPLY(this->prefix.hostname, clients, channel_name, this->prefix.nickname));
    DataControler::SendClientMessage(clientID,RPL_ENDOFNAMES(this->prefix.hostname, this->prefix.nickname, channel_name));
}

/*
    user reach the max number of channels -> ERR_TOOMANYCHANNELS -> 1
    channelname length is less than 50 -> ERR_BADCHANMASK -> 2
*/
void JoinCommand::joinNewChannel(const std::string& channel) {
    Clients *cl = DataControler::getClient(clientID);
    if (cl->getJoinedChannels().size() >= CCHANLIMIT)
        return(DataControler::SendClientMessage(clientID,ERR_TOOMANYCHANNELS(this->prefix.nickname, this->prefix.hostname)));
    if (channel.length() > 50)
        return(DataControler::SendClientMessage(clientID,ERR_BADCHANMASK(this->prefix.nickname, this->prefix.hostname)));
    Channels *ch = DataControler::addChannel(channel);
    ch->addClientIn(1, clientID);
    cl->joinChannel(DataControler::transformCase(channel));
    DataControler::SendClientMessage(clientID,RPL_JOIN(this->prefix.nickname, this->prefix.username, channel, this->prefix.hostname));
}
/*
	REPLAY::

	ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
    ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
    ERR_CHANNELISFULL               ERR_BADCHANMASK
    ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
    ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
    RPL_TOPIC

	COMMAND SYNTAX

	JOIN <channel>{,<channel>} [<key>{,<key>}]

*/
void JoinCommand::execute() {

    std::istringstream stream(this->message);
    std::string part;
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    Clients *cl = DataControler::getClient(this->clientID);
    // size_t prev_pos = 0;
    // int count = 0;

	if (this->message.empty())
		return (DataControler::SendClientMessage(clientID,ERR_NEEDMOREPARAMS(this->prefix.nickname, this->prefix.hostname)));
    if (cl->getRegistrationStatus() == 1){
        // std::string nickname = cl->getNickName();
        return (DataControler::SendClientMessage(this->clientID, ERR_NOTREGISTERED(this->prefix.nickname, this->prefix.hostname)));
    }

	// Split the message into parts vector of channels and vector of keys if any additional will be saved in ignored
	std::vector<std::string> parts = splitParts(this->message, 2);
	if (parts.size() >= 1) {
		channels = split(parts[0], ',');
        if (parts.size() >= 2)
			keys = split(parts[1], ',');
		// if (!parts[1].empty())
	}

	for (size_t i = 0; i < channels.size(); i++) {
		if (!checkTokenChar(channels[i], 1))
			DataControler::SendClientMessage(clientID,ERR_BADCHANNELNAME(this->prefix.nickname, this->prefix.hostname, channels[i]));
		else{
		channels[i] = channels[i].substr(1);
			if (DataControler::channelnamesExist(channels[i])){
                if (keys.size() > i)
                    joinExistingChannel(channels[i], keys[i]);
                else
                    joinExistingChannel(channels[i], "");
            }
			else
				joinNewChannel(channels[i]);
		}
	}
}

//-----------------------------------------------------------

PassCommand::PassCommand() : Command(0) {
    this->command = PASS;
}

PassCommand::PassCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = PASS;
    this->password = message;
}

void PassCommand::execute() {
    std::cout << "PASS is " << this->password << std::endl;
    Clients *cl = DataControler::getClient(clientID);
    if (cl->getRegistrationStatus() >= 1)
        // std::cout << "Already Registered" << std::endl;
        return (DataControler::SendClientMessage(clientID, ERR_ALREADYREGISTERED(" ", prefix.hostname)));
    if (this->password.empty())
        // std::cout << "Password Empty" << std::endl;
        return (DataControler::SendClientMessage(clientID, ERR_NEEDMOREPARAMS(" ", prefix.hostname)));
    if (!DataControler::PasswordCheck(this->password))
        // std::cout << "Password NOT Matched" << std::endl;
        return (DataControler::SendClientMessage(clientID, ERR_PASSWDMISMATCH(" ", prefix.hostname)));
    cl->setRegistrationStatus(1);
    DataControler::SendClientMessage(clientID, RPL_WELCOME(prefix.nickname, prefix.hostname));
}

PassCommand::~PassCommand() {}


//-----------------------------------------------------------

NickCommand::NickCommand() : Command(0) {
    this->command = NICK;
}

NickCommand::~NickCommand() {}

NickCommand::NickCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = NICK;
    this->nickname = message;
}

void NickCommand::execute() {
    Clients *cl = DataControler::getClient(clientID);
    if (nickname.empty())
        return (DataControler::SendClientMessage(clientID, ERR_NONICKNAMEGIVEN(prefix.nickname, prefix.hostname)));
    if (!checkTokenChar(this->nickname, 2))
        return (DataControler::SendClientMessage(clientID, ERR_ERRONEUSNICKNAME(prefix.nickname, prefix.hostname)));
    if (!DataControler::nicknamesUnique(this->nickname))
        return (DataControler::SendClientMessage(clientID, ERR_NICKNAMEINUSE(prefix.nickname, prefix.hostname)));
    cl->setNickName(this->nickname);
}

//-----------------------------------------------------------

UnknownCommand::UnknownCommand() : Command(0) {
    this->command = UNKNOWN;
}

UnknownCommand::~UnknownCommand() {}

UnknownCommand::UnknownCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = UNKNOWN;
    this->message = message;
}

void UnknownCommand::execute() {
    DataControler::SendClientMessage(clientID, ERR_UNKNOWNCOMMAND(prefix.nickname, prefix.hostname, message));
}

//-----------------------------------------------------------


QuitCommand::QuitCommand() : Command(0) {
    this->command = QUIT;
}

QuitCommand::QuitCommand(int clientID, const std::string& _messag) : Command(0){
    this->clientID = clientID;
    this->command = QUIT;
    this->message = _messag;
}

void QuitCommand::execute(){
    Clients *cl = DataControler::getClient(this->clientID);
    if (cl->getJoinedChannels().size() > 0){
        std::vector<std::string> _joinedchannels = cl->getJoinedChannels();
        for (std::vector<std::string>::iterator it = _joinedchannels.begin(); it != _joinedchannels.end(); it++)
        {
            if(DataControler::getChannel(*it)->isOperator(this->clientID))
            {
                if(DataControler::getChannel(*it)->getList(1).size() == 1)
                {
                    DataControler::removeChannel(*it);
                }else if(DataControler::getChannel(*it)->getList(1).size() > 1){
                    // DataControler::getChannel(*it)->removeClientFrom(1, this->clientID);
                    DataControler::getClient(clientID)->leaveChannel(*it);
                }
            }else if(DataControler::getChannel(*it)->isMember(this->clientID))
                DataControler::getClient(clientID)->leaveChannel(*it);
        }
        DataControler::removeClient(this->clientID);
    }
}

QuitCommand::~QuitCommand() {}



// -----------------------------------------------------


UserCommand::UserCommand() : Command(0) {};
UserCommand::UserCommand(int clientid, const std::string& _message) : Command(clientid){
    this->command = USER;
    this->message = _message;
}
UserCommand::~UserCommand(){}

// userName 0 * :realname
void UserCommand::execute(){
    std::vector<std::string> parts;
    parts = splitParts(this->message, 4);

    if (parts.size() < 4)
        return (DataControler::SendClientMessage(clientID,ERR_NEEDMOREPARAMS(this->prefix.nickname, this->prefix.hostname)));
    if (parts[1] != "0" || parts[2] != "*" || !checkTokenChar(parts[0],3))
        return;
    Clients *cl = DataControler::getClient(clientID);
    if (cl->getRegistrationStatus() == 3)
        return (DataControler::SendClientMessage(clientID,ERR_ALREADYREGISTERED(this->prefix.nickname, this->prefix.hostname)));
    cl->setRealName(parts[3]);
    cl->setUserName(parts[0]);
    cl->setRegistrationStatus(3);
}


// -----------------------------------------------------


ModeCommand::ModeCommand() : Command(0) {};
ModeCommand::ModeCommand(int clientid, const std::string& _message) : Command(clientid){
    this->command = MODE;
    this->message = _message;
}

/*
    message : 
    i t k o l
    MODE #channelname +i // add 
    MODE #channelname -i // remove

    #channelname +i 
    #channelname +t
    #channelname +k password
    #channelname +o nickname
    #channelname +l limit

    #channelname -i
    #channelname -t
    #channelname -k
    #channelname -o nickname
    #channelname -l

    #channelname 
*/
void ModeCommand::execute(){
    std::vector<std::string> parts;

    parts = splitParts(this->message, 3); // channelname modestring modeparam
}

ModeCommand::~ModeCommand
