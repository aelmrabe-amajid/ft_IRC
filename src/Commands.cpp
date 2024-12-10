#include "../inc/Commands.hpp"
#include <iostream>
#include <bitset>


std::string Command::UPREF(int _clientID){
    Clients *cl;

    cl = DataControler::getClient(_clientID);
    return (
        ":" +
        cl->getNickName() + 
        "!" +
        cl->getUserName() +
        "@" + 
        cl->getHostName()
    );
}


static CommandID getCommandId(const std::string& command) {
    if (command == "PASS")          return PASS;
    if (command == "NICK")          return NICK;
    if (command == "USER")          return USER;
    if (command == "JOIN")          return JOIN;
    if (command == "MODE")          return MODE;
    if (command == "QUIT")          return QUIT;
    if (command == "PART")          return PART;
    if (command == "INVITE")        return INVITE;
    if (command == "KICK")          return KICK;
    if (command == "PRIVMSG")       return PRIVMSG;
    if (command == "TOPIC")         return TOPIC;
    if (command == "PONG")          return PONG;
    return UNKNOWN;
}

static std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

static std::vector<std::string> splitParts(const std::string& str, int parts) {
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
};

std::string Command::transformCase(std::string &str) {
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = static_cast<char>(std::toupper(str[i]));
    }
    return str;
}

static std::vector<std::string> parse(const std::string& _message) {
    std::string msg = _message;
    std::vector<std::string> parse;
    std::string commad;
	size_t pos = 0;

    if (msg[msg.size() - 1] == '\n' || msg[msg.size() - 1] == '\r')
        msg.pop_back();
	while (pos < msg.size() && msg[pos] == ' ')
        pos++;
    // if (msg[0] != '/')
    //     return;
    size_t command_end = msg.find(' ', pos);
    if (command_end == std::string::npos) {
        commad = msg.substr(pos);
        pos = msg.size();
    } else {
        commad = msg.substr(pos, command_end - pos);
        pos = command_end;
    }
    if (commad[0] == '/')
        commad = commad.substr(1); // remove the /
    parse.push_back(Command::transformCase(commad)); // Convert to Upper
	while (pos < msg.size() && msg[pos] == ' ')
		pos++;
    if (msg.length() == pos) {
        return parse;
    }
	parse.push_back(msg.substr(pos));
    return (parse);
}

void Command::HandleCommand(int clientID, std::vector<std::string>& message) {
    std::string password;
    std::string nickname;
    std::string _user;
    for (std::vector<std::string>::iterator it = message.begin(); it != message.end(); ++it) {
        std::vector<std::string> msg;
        if ((*it).empty()){
            std::string client = "*";
            DataControler::SendMsg(clientID,ERR_UNKNOWNCOMMAND(client," "));
            DataControler::removeClient(clientID,false);
            return;
        }
        msg = parse(*it);
        CommandID commandId = getCommandId(msg[0]);
        switch (commandId) {
            case PASS:
                password = msg[1];
                break;
            case NICK:
                nickname = msg[1];
                break;
            case USER:
                _user = msg[1];
                break;
            default:
                DataControler::removeClient(clientID,false);
                return;
        }
    }
    Command *cmd;
    try {
        cmd = Command::createCommand(clientID, password, PASS);
        cmd->execute();
        delete cmd;
    }catch (std::exception &e) {
        DataControler::removeClient(clientID,false);
        delete cmd;
        return;
    }
    try {
        cmd = Command::createCommand(clientID, nickname, NICK);
        cmd->execute();
        delete cmd;
    }catch (std::exception &e){
        DataControler::removeClient(clientID,false);
        delete cmd;
        return;
    }
    try {
        cmd = Command::createCommand(clientID, _user, USER);
        cmd->execute();
        delete cmd;
    }catch (std::exception &e){
        DataControler::removeClient(clientID,false);
        delete cmd;
        return;
    }
    Clients *cl = DataControler::getClient(clientID);
    cl->setRegistrationStatus(1);
    // DataControler::SendMsg(clientID,RPL_WELCOME(UPRF(clientID),cl->getNickName()));
    // std::cout <<  << std:
    std::string prefix = Command::UPREF(clientID);
    // DataControler::SendMsg(clientID,RPL_WELCOME(user_id(cl->getNickName(),cl->getUserName()),cl->getNickName()));
    DataControler::SendMsg(clientID,RPL_WELCOME(prefix,cl->getNickName()));
    DataControler::SendMsg(clientID,RPL_YOURHOST(cl->getNickName(),"localhost","1.0"));
    DataControler::SendMsg(clientID,RPL_CREATED(cl->getNickName(),DataControler::serverCreationDate()));
    DataControler::SendMsg(clientID,RPL_MYINFO(cl->getNickName(),"localhost","1.0","ol","itkol","lok"));
    DataControler::SendMsg(clientID,RPL_ISUPPORT(cl->getNickName()," CASEMAPPING=rfc1459 CHANLIMIT=#:200 CHANMODES=k,l,ti CHANNELLEN=200 CHANTYPES=# NICKLEN=9 PREFIX=(o)@+ USERLEN=9 TOPICLEN=255"));
}

void Command::HandleCommand(int clientID, const std::string& _message) {
    std::vector<std::string> message = parse(_message);

    if (DataControler::isClient(clientID) == false)
        return;
    if (DataControler::getClient(clientID)->getRegistrationStatus() == 0)
        return (HandleCommand(clientID, message));
    if (message.size() == 0)
        return (DataControler::SendMsg(clientID,ERR_UNKNOWNCOMMAND(DataControler::getClientNickname(clientID)," ")));
    CommandID commandId = getCommandId(message[0]);
    if (commandId == PONG)
        return;
	if (commandId == UNKNOWN)
        return (DataControler::SendMsg(clientID,ERR_UNKNOWNCOMMAND(DataControler::getClientNickname(clientID),message[0])));
    if (commandId == USER || commandId == PASS)
        return (DataControler::SendMsg(clientID,ERR_ALREADYREGISTERED(DataControler::getClientNickname(clientID))));
    if (message.size() == 1)
        message.push_back("");
	Command *cmd = Command::createCommand(clientID, message[1], commandId);
    cmd->execute();
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
        case MODE:
            cmd = new ModeCommand(clientID, params);
            break;
        case PART:
            cmd = new PartCommand(clientID, params);
            break;
        case INVITE:
            cmd = new InviteCommand(clientID, params);
            break;
        case KICK:
            cmd = new KickCommand(clientID, params);
            break;
        case TOPIC:
            cmd = new TopicCommand(clientID, params);
            break;
        case PRIVMSG:
            cmd = new PrivmsgCommand(clientID, params);
            break;
        default:
            cmd = new UnknownCommand(clientID, params);
            break;
	}
	return cmd;
}

static bool checkTokenChar(const std::string& name, int type) {
	size_t len = name.length();
    if (type == 1 && name[0] != '#')
        return false;
	if ((type == 2 || type == 3) && !isalpha(name[0]))
		return false;
    for (size_t i = 1; i < len; ++i) {
        char c = name[i];
        if (type == 1 && c == '#')
            continue;
        else if (!(
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

//-----------------------------------------------------------

PassCommand::PassCommand() : Command(0) {
    this->command = PASS;
}

PassCommand::PassCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = PASS;
    this->psw = message;
}

void PassCommand::execute() {
    Clients *cl = DataControler::getClient(clientID);
    std::vector<std::string> parts;
    std::string cl_n = "*";
    if (cl->getRegistrationStatus() == 1)
        return (DataControler::SendMsg(clientID,ERR_ALREADYREGISTERED(DataControler::getClientNickname(clientID))));
    if (this->psw.empty())
        return (DataControler::SendMsg(clientID,ERR_NEEDMOREPARAMS(cl_n,"PASS")),throw std::invalid_argument(""));
    parts = split(this->psw, ' ');
    if (DataControler::PasswordCheck(this->psw) == false)
        return (DataControler::SendMsg(clientID,ERR_PASSWDMISMATCH(cl_n)),throw std::invalid_argument(""));
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
    std::cout << "im here" << std::endl;
    if (cl->getRegistrationStatus() == 0){
        std::string cl_n = "*";
        if (nickname.empty())
            return (DataControler::SendMsg(clientID, ERR_NONICKNAMEGIVEN(cl_n)), throw std::invalid_argument(""));
        if (!checkTokenChar(this->nickname, 2))
            return (DataControler::SendMsg(clientID, ERR_ERRONEUSNICKNAME(cl_n,nickname)), throw std::invalid_argument(""));
        if (DataControler::nicknamesUnique(this->nickname) == false)
            return (DataControler::SendMsg(clientID, ERR_NICKNAMEINUSE(cl_n,nickname)), throw std::invalid_argument(""));
        cl->setNickName(this->nickname);
    }
    else {
        if (nickname.empty())
            return (DataControler::SendMsg(clientID, ERR_NONICKNAMEGIVEN(cl->getNickName())));
        if (!checkTokenChar(this->nickname, 2))
            return (DataControler::SendMsg(clientID, ERR_ERRONEUSNICKNAME(cl->getNickName(),this->nickname)));
        if (!DataControler::nicknamesUnique(this->nickname))
            return (DataControler::SendMsg(clientID, ERR_NICKNAMEINUSE(cl->getNickName(),this->nickname)));
        DataControler::SendMsg(clientID,RPL_NICK(cl->getNickName(),cl->getNickName(),this->nickname));
        std::vector<std::string> joinedChannels = cl->getJoinedChannels();

        for (std::vector<std::string>::iterator it = joinedChannels.begin(); it != joinedChannels.end(); ++it) {
            Channels *ch = DataControler::getChannel(*it);
            if (ch->isMember(clientID) == true)
                DataControler::SendMsg(ch->getChannelName(),RPL_NICK(cl->getNickName(),cl->getNickName(),this->nickname));
        }
        cl->setNickName(this->nickname);
    }
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
    return;
}

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
    Clients *cl;

    cl = DataControler::getClient(this->clientID);
    if (cl->getRegistrationStatus() == 1)
        return (DataControler::SendMsg(clientID,ERR_ALREADYREGISTERED(cl->getNickName())));
    if (this->message.empty())
        return (DataControler::SendMsg(clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"USER")),throw std::invalid_argument(""));
    parts = splitParts(this->message, 4);
    if (parts.size() < 4)
        return (DataControler::SendMsg(clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"USER")),throw std::invalid_argument(""));
    if (parts[1] != "0" || parts[2] != "*" || !checkTokenChar(parts[0],3))
        return;
    cl->setRealName(parts[3]);
    cl->setUserName(parts[0]);
}

//----------------------------------------------------------
JoinCommand::~JoinCommand() {}
JoinCommand::JoinCommand() : Command(0) {
    this->command = JOIN;
}

JoinCommand::JoinCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = JOIN;
    this->message = message;
};

void JoinCommand::joinExistingChannel(const std::string& channel, const std::string& key) {
    Channels *ch = DataControler::getChannel(channel);
    Clients *cl = DataControler::getClient(clientID);
    
    if (cl->isClientInChannel(channel))
        return;
    // Remember to remove this option of BANNED
    if (ch->isBanned(clientID))
        return (DataControler::SendMsg(clientID,ERR_BANNEDFROMCHAN(cl->getNickName(),ch->getChannelName())));
    if (ch->isFull())
        return (DataControler::SendMsg(clientID,ERR_CHANNELISFULL(cl->getNickName(),ch->getChannelName())));
    if (cl->getJoinedChannels().size() >= CHANLIMIT)
        return;
    if (!ch->isPublic() && !ch->isInvited(clientID))
        return (DataControler::SendMsg(clientID,ERR_INVITEONLYCHAN(cl->getNickName(),ch->getChannelName())));
    // if (!ch->isPublic() && !ch->isInvited(clientID))
    //     return (DataControler::SendMsg(clientID,ERR_INVITEONLYCHAN(cl->getNickName(),ch->getChannelName())));
    if (ch->isSecret() && !ch->isKeyValid(key))
        return (DataControler::SendMsg(clientID,ERR_BADCHANNELKEY(cl->getNickName(),ch->getChannelName())));    
    ch->addClientIn(0, clientID);
    cl->joinChannel(channel);
    if (!ch->isPublic())
        ch->RmInvite(0,clientID,1);
    // if (ch->InviteList[])
    DataControler::SendMsg(ch->getChannelName(),RPL_JOIN(user_id(cl->getNickName(),cl->getUserName()),ch->getChannelName()));
    DataControler::SendMsg(clientID,RPL_NAMREPLY(cl->getNickName(),"@",ch->getChannelName(),ch->getMemberList()));
    DataControler::SendMsg(clientID,RPL_ENDOFNAMES(cl->getNickName(),ch->getChannelName()));
    if (ch->getTopic().size() > 0)
        DataControler::SendMsg(clientID,RPL_TOPIC(cl->getNickName(),ch->getChannelName(),ch->getTopic()));
};

void JoinCommand::joinNewChannel(const std::string& channel) {
    Clients *cl = DataControler::getClient(clientID);
    if (cl->getJoinedChannels().size() >= CHANLIMIT){
        DataControler::SendMsg(clientID,ERR_TOOMANYCHANNELS(cl->getNickName()));
        return;
    }
    Channels *ch = DataControler::addChannel(channel);
    ch->addClientIn(1, clientID);
    cl->joinChannel(DataControler::transformCase(channel));
    DataControler::SendMsg(clientID,RPL_JOIN(user_id(cl->getNickName(),cl->getUserName()),ch->getChannelName()));
    DataControler::SendMsg(clientID,MODE_CHANNELMSG(ch->getChannelName(),"+o " + cl->getNickName()));
}

void JoinCommand::execute() {

    std::istringstream stream(this->message);
    std::string part;
    Clients *cl = DataControler::getClient(this->clientID);

	if (this->message.empty())
        return (DataControler::SendMsg(clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"JOIN")));
	std::vector<std::string> parts = splitParts(this->message, 2);
	if (parts.size() >= 1) {
		channels = split(parts[0], ',');
        if (parts.size() >= 2)
			keys = split(parts[1], ',');
	}
	for (size_t i = 0; i < channels.size(); i++) {
        if (channels[i] == "#0"){
            cl->leaveChannel();
            continue;
        }
        if (!checkTokenChar(channels[i], 1) ||  channels[i].length() < 2){
            if (channels[i][0] != '#')
                DataControler::SendMsg(clientID,ERR_BADCHANMASK(cl->getNickName(),channels[i]));
            else
                DataControler::SendMsg(clientID,ERR_BADCHANMASK(cl->getNickName(),channels[i].erase(0,1)));
        }
        else {
            channels[i].erase(0,1);
            if (channels[i].size() > CHANNELLEN){
                DataControler::SendMsg(clientID, ERR_CHNAME(cl->getNickName(), channels[i]));
                continue;
            }
            if(DataControler::channelnamesExist(channels[i])){
                if (keys.size() > 1)
                    joinExistingChannel(channels[i],keys[i]);
                else
                    joinExistingChannel(channels[i],"");
            }
            else{
                if (channels[i].size() > 0)
                    channels[i] = channels[i];
                joinNewChannel(channels[i]);
            }
        }
	}
}
//-----------------------------------------------------------

ModeCommand::ModeCommand() : Command(0) {
    this->command = MODE;
}

ModeCommand::~ModeCommand() {}

ModeCommand::ModeCommand(int clientID, const std::string& _message) : Command(clientID) {
    this->command = MODE;
    this->message = _message;
};

void ModeCommand::fillModeInfos(){
    size_t  pos;
	pos = this->message.find(" ");
	if (pos == std::string::npos)
	{
        this->target = this->message.substr(0);
		return ;
	}
	else
	{
        this->target = this->message.substr(0, pos);
        this->message.erase(0, pos + 1);
	}
	pos = this->message.find(" ");
	if (pos == std::string::npos)
	{
		this->mode = this->message.substr(0);
		return;
	}
	else
	{
		this->mode = this->message.substr(0, pos);
		this->message.erase(0, pos + 1);
	}

    while (!this->message.empty()) {
        pos = this->message.find(' ');
        if (pos == std::string::npos) {
            this->params.push_back(this->message);
            break;
        }
        std::string param = this->message.substr(0, pos);
        if (param[0] == ':') {
            this->params.push_back(this->message);
            break;
        }
        this->params.push_back(param);
        this->message.erase(0, pos + 1);
    }
}

static bool isvalid(std::string &parm, int ty){
    if (ty == 0){
        for (size_t i = 0; i < parm.size(); i++){
            if (!isnumber(static_cast<int>(parm[i])))
                return (false);
        };
        return (true);
    }
    for (size_t i = 0; i < parm.size(); i++){
        if (!isalpha(static_cast<int>(parm[i])))
            return (false);
        };
    return (true);
}

void ModeCommand::execute(){
    Clients *cl = DataControler::getClient(clientID);

    fillModeInfos();
    if (this->target[0] != '#')
        return (DataControler::SendMsg(clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),this->target)));
    std::string channel = this->target.erase(0,1);
    if (DataControler::channelnamesExist(channel) == false)
        return (DataControler::SendMsg(clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),this->target)));
    Channels *ch = DataControler::getChannel(channel);
    if (this->mode.empty() == true){
        if (ch->isMember(clientID) == false && ch->isOperator(clientID) == false)
            return (DataControler::SendMsg(clientID,ERR_NOTONCHANNEL(cl->getNickName(),ch->getChannelName())));
        return (DataControler::SendMsg(clientID,RPL_CHANNELMODEIS(cl->getNickName(),ch->getChannelName(),ch->getChannelModes())));
    }
    if (ch->isOperator(clientID) == false)
        return (DataControler::SendMsg(clientID,ERR_CHANOPRIVSNEEDED(cl->getNickName(),ch->getChannelName())));
    std::vector<ModeInfo> modes;
    bool set = true;
	size_t arg = 0;
    std::string s_mode;
    if (!this->mode.empty()){
        for (size_t i = 0; i < this->mode.size(); i++) {
            if (this->mode[i] == '+') {
                set = true;
                continue;
            } else if (this->mode[i] == '-') {
                set = false;
                continue;
            }
            ModeInfo mode_info;
            mode_info.set = set;
            mode_info.type = mode[i];
            if ((mode[i] == 'k' || mode[i] == 'o' || mode[i] == 'l') && set && arg < params.size())
                mode_info.param = params[arg++];
            else if (mode[i] == 'o' && !set && arg < params.size())
                mode_info.param = params[arg++];
            modes.push_back(mode_info);
        };
    }
    for (size_t i = 0; i < modes.size() && i < MAXMODS; i++) {
        char c = static_cast <char>(modes[i].type);
        (modes[i].set ? s_mode = "+"  : s_mode = "-" );
        s_mode += c;
        if (modes[i].type == 'i' || modes[i].type == 't'){
            ch->setChannelMode(c, modes[i].set);
            DataControler::SendMsg(ch->getChannelName(),MODE_CHANNELMSG(ch->getChannelName(),s_mode));
        }
        else if (modes[i].type == 'k'){
            if (modes[i].set){
                if (modes[i].param.empty() || modes[i].param.size() > KEYLEN || !isvalid(modes[i].param, 1)){
                    if (modes[i].param.empty())
                        modes[i].param = "";
                    DataControler::SendMsg(clientID, ERR_INVALIDKEY(cl->getNickName(), ch->getChannelName()));
                    continue;
                } 
            }
            ch->setChannelMode(c, modes[i].set, modes[i].param);
            DataControler::SendMsg(ch->getChannelName(),MODE_CHANNELMSG(ch->getChannelName(),s_mode));
        }
        else if (modes[i].type == 'l'){
            if (modes[i].set){
                if (modes[i].param.empty() || std::stoi(modes[i].param) < 0 || !isvalid(modes[i].param,0))
                    continue;
                if (std::stoi(modes[i].param) < ch->getMemberLimit())
                    modes[i].param = std::to_string(ch->getMemberLimit());
            }
            ch->setChannelMode(c, modes[i].set, modes[i].param);
            DataControler::SendMsg(ch->getChannelName(),MODE_CHANNELMSG(ch->getChannelName(),s_mode + " " + modes[i].param));
        }
        else if (modes[i].type == 'o'){
            if (modes[i].param.empty())
                continue;
            if (DataControler::isClient(modes[i].param) == false){
                DataControler::SendMsg(this->clientID, ERR_NOSUCHNICK(cl->getNickName(),modes[i].param));
                continue;
            }
            int id = DataControler::getClient(modes[i].param)->getID();
            if (ch->isMember(id) == false && ch->isOperator(id) == false){
                DataControler::SendMsg(this->clientID, ERR_NOTONCHANNEL(cl->getNickName(),ch->getChannelName()));
                continue;
            }
            if (modes[i].set){
                if (ch->isOperator(id) == true)
                    continue;
                ch->addClientIn(1, id);
                ch->removeClientFrom(0,id);
                DataControler::SendMsg(ch->getChannelName(),MODE_CHANNELMSG(ch->getChannelName(),s_mode + " " + modes[i].param));
            }
            else if (modes[i].set == false){
                if (ch->isOperator(id) == false)
                    continue;
                if (ch->getList(1).size() == 1)
                    continue;
                ch->RmInvite(id,0,2);
                ch->removeClientFrom(1,id);
                ch->addClientIn(0,id);
                DataControler::SendMsg(ch->getChannelName(),MODE_CHANNELMSG(ch->getChannelName(),s_mode + " " + modes[i].param));
            }
        }
        else
            DataControler::SendMsg(this->clientID, ERR_UNKNOWNMODE(cl->getNickName(), s_mode));
    }
}

//-----------------------------------------------------------

QuitCommand::QuitCommand() : Command(0) {};

QuitCommand::~QuitCommand() {}

QuitCommand::QuitCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = QUIT;
    this->message = message;
}

void QuitCommand::execute() {
    Clients *cl = DataControler::getClient(clientID);
    std::vector<Channels*> channels;
    std::vector<std::string> joinedChannels = cl->getJoinedChannels();
    std::string reason;
    if (this->message.empty())
        reason = "leaving ...";
    else
        reason = this->message;
    cl->PendingInvite("",clientID,5);
    if (joinedChannels.size() > 0){
        for (std::vector<std::string>::iterator it = joinedChannels.begin(); it != joinedChannels.end(); ++it) {
            Channels *ch = DataControler::getChannel(*it);
            channels.push_back(ch);
        }
        for (size_t i = 0; i < channels.size(); i++){
            if (channels[i]->isOperator(clientID) == true){
                std::vector<int> members = channels[i]->getList(0);
                std::string n_reason = ":No operator left in the channel";
                std::string ch_n = channels[i]->getChannelName();
                for (std::vector<int>::iterator it = members.begin(); it != members.end(); ++it) {
                    Clients *_cl = DataControler::getClient(*it);
                    DataControler::SendMsg(_cl->getID(),RPL_PART(user_id(_cl->getNickName(),_cl->getUserName()),ch_n,n_reason));
                    _cl->leaveChannel(channels[i]->getChannelName());
                }
                channels[i]->RmInvite(0,0,3);
                DataControler::removeChannel(channels[i]->getChannelName());
            }
            else if (channels[i]->isMember(clientID) == true){
                    DataControler::SendMsg(channels[i]->getChannelName(),RPL_PART(user_id(cl->getNickName(),cl->getUserName()),channels[i]->getChannelName(),reason));
                    channels[i]->removeClientFrom(channels[i]->isOperator(clientID),clientID);
            }
            DataControler::SendMsg(clientID,RPL_QUIT(cl->getNickName(),reason));
            DataControler::removeClient(clientID,false);
        }
    }
}

//-----------------------------------------------------------

PartCommand::PartCommand() : Command(0) {};

PartCommand::~PartCommand() {}

PartCommand::PartCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = PART;
    this->message = message;
}

void PartCommand::execute() {
    std::vector<std::string> parts;
    Clients *cl = DataControler::getClient(clientID);
    if (this->message.empty() == true)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"PART")));
    parts = splitParts(this->message, 2);
    this->channels = split(parts[0], ',');
    if (parts.size() >= 2)
        this->reason = parts[1];
    else
        this->reason = "Leaving ...";
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i][0] != '#' || DataControler::channelnamesExist(this->channels[i].substr(1,this->channels[i].length())) == false)
            return (DataControler::SendMsg(this->clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),this->channels[i])));
        Channels *ch = DataControler::getChannel(this->channels[i].erase(0,1));
        if (ch->isMember(clientID) == false && ch->isOperator(clientID) == false)
            return (DataControler::SendMsg(this->clientID,ERR_NOTONCHANNEL(cl->getNickName(),ch->getChannelName())));
        if (ch->getMemberCount() == 1) {
            DataControler::SendMsg(this->clientID,RPL_PART(user_id(cl->getNickName(),cl->getUserName()),ch->getChannelName(),this->reason));
            cl->leaveChannel(ch->getChannelName());
            return (DataControler::removeChannel(ch->getChannelName()));
        }
        else{
            if (ch->getList(1).size() == 1 && ch->isOperator(clientID) == true) {
                DataControler::SendMsg(ch->getChannelName(),RPL_PART(user_id(cl->getNickName(),cl->getUserName()),ch->getChannelName(),this->reason));
                std::vector<int> members = ch->getList(0);
                std::string n_reason = ":No operator left in the channel";
                std::string ch_n = ch->getChannelName();
                for (std::vector<int>::iterator it = members.begin(); it != members.end(); ++it) {
                    Clients *_cl = DataControler::getClient(*it);
                    DataControler::SendMsg(_cl->getID(),RPL_PART(user_id(_cl->getNickName(),_cl->getUserName()),ch_n,n_reason));
                    _cl->leaveChannel(ch->getChannelName());
                }
                ch->RmInvite(0,0,3);
                cl->leaveChannel(ch->getChannelName());
                DataControler::removeChannel(ch->getChannelName());
                return;
            }
            ch->RmInvite(clientID,0,2);
            DataControler::SendMsg(ch->getChannelName(),RPL_PART(user_id(cl->getNickName(),cl->getUserName()),ch->getChannelName(),this->reason));
            ch->removeClientFrom(ch->isOperator(clientID),clientID);
            cl->leaveChannel(ch->getChannelName());
        }
    }
}

//-----------------------------------------------------------

InviteCommand::InviteCommand() : Command(0) {};

InviteCommand::~InviteCommand() {}

InviteCommand::InviteCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = INVITE;
    this->message = message;
}

void InviteCommand::execute() {
    std::vector<std::string> parts;
    Clients *cl = DataControler::getClient(clientID);
    if (this->message.empty() == true)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"INVITE")));
    parts = splitParts(this->message, 2);
    if (parts.size() < 2)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"INVITE")));
    if (DataControler::isClient(parts[0]) == false)
        return (DataControler::SendMsg(this->clientID,ERR_NOSUCHNICK(cl->getNickName(),parts[0])));
    if (parts[1][0] != '#' || DataControler::channelnamesExist(parts[1].substr(1,parts[1].length())) == false)
        return (DataControler::SendMsg(this->clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),parts[1])));
    Channels *ch = DataControler::getChannel(parts[1].erase(0,1));
    if (ch->isMember(clientID) == false && ch->isOperator(clientID) == false)
        return (DataControler::SendMsg(this->clientID,ERR_NOTONCHANNEL(cl->getNickName(),ch->getChannelName())));
    if (ch->isPublic() == false && ch->isOperator(clientID) == false)
        return (DataControler::SendMsg(this->clientID,ERR_CHANOPRIVSNEEDED(cl->getNickName(),ch->getChannelName())));
    if (ch->isMember(DataControler::getClient(parts[0])->getID()))
        return (DataControler::SendMsg(this->clientID,ERR_USERONCHANNEL(cl->getNickName(),parts[0],ch->getChannelName())));
    ch->AddInvite(clientID,DataControler::getClient(parts[0])->getID());
    DataControler::SendMsg(parts[0],RPL_INVITING(user_id(cl->getNickName(),cl->getUserName()),cl->getNickName(),parts[0],ch->getChannelName()));
}

//-----------------------------------------------------------


KickCommand::KickCommand() : Command(0) {};
KickCommand::~KickCommand() {};
KickCommand::KickCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = KICK;
    this->message = message;
};


void KickCommand::execute(){
    std::vector<std::string> parts;
    Clients *cl = DataControler::getClient(clientID);
    if (this->message.empty() == true)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"KICK")));
    parts = splitParts(this->message, 3);
    if (parts.size() < 2)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"KICK")));
    if (parts[0][0] != '#')
        return (DataControler::SendMsg(this->clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),parts[0])));
    parts[0].erase(0,1);
    if (DataControler::channelnamesExist(parts[0]) == false)
        return (DataControler::SendMsg(this->clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),parts[0])));
    Channels *ch = DataControler::getChannel(parts[0]);
    if (ch->isMember(clientID) == false && ch->isOperator(clientID) == false)
        return (DataControler::SendMsg(this->clientID,ERR_NOTONCHANNEL(cl->getNickName(),ch->getChannelName())));
    if (ch->isOperator(clientID) == false)
        return (DataControler::SendMsg(this->clientID,ERR_CHANOPRIVSNEEDED(cl->getNickName(),ch->getChannelName())));
    std::vector<std::string> users = split(parts[1], ',');
    std::string comment = parts[2];
    if (comment.empty())
        comment = "No Reason ...";
    Clients *_cl;
    for (size_t i = 0; i < users.size(); i++) {
        if (DataControler::isClient(users[i]) == false)
            return (DataControler::SendMsg(this->clientID,ERR_NOSUCHNICK(cl->getNickName(),users[i])));
        _cl = DataControler::getClient(users[i]);
        if (ch->isMember(_cl->getID()) == false && ch->isOperator(_cl->getID()) == false)
            return (DataControler::SendMsg(this->clientID,ERR_USERNOTINCHANNEL(cl->getNickName(),users[i],ch->getChannelName())));
        if (_cl->getID() == clientID)
            return;
        if (ch->isMember(_cl->getID())){
            ch->removeClientFrom(0,_cl->getID());
            _cl->leaveChannel(ch->getChannelName());
        }
        else if (ch->isOperator(_cl->getID())){
            ch->RmInvite(clientID,0,2);
            _cl->leaveChannel(ch->getChannelName());
            // ch->RemovePendingInvitesFrom(_cl->getID());
            ch->removeClientFrom(1,_cl->getID());
        }
        // DataControler::SendMsg(_cl->getID(),RPL_KICK(user_id(cl->getNickName(),cl->getUserName()),ch->getChannelName(),users[i],comment));
        DataControler::SendMsg(ch->getChannelName(),RPL_KICK(user_id(cl->getNickName(),cl->getUserName()),ch->getChannelName(),users[i],comment));
    }
}

//-----------------------------------------------------------

TopicCommand::TopicCommand() : Command(0) {};
TopicCommand::~TopicCommand() {};
TopicCommand::TopicCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = TOPIC;
    this->message = message;
};

void TopicCommand::execute(){
    std::vector<std::string> parts;
    Clients *cl = DataControler::getClient(clientID);
    if (this->message.empty() == true)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"TOPIC")));
    parts = splitParts(this->message, 2);
    if (parts.size() < 1)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"TOPIC")));
    if (parts[0][0] != '#')
        return (DataControler::SendMsg(this->clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),parts[0])));
    parts[0].erase(0,1);
    if (DataControler::channelnamesExist(parts[0]) == false)
        return (DataControler::SendMsg(this->clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),parts[0])));
    Channels *ch = DataControler::getChannel(parts[0]);
    if (ch->isMember(clientID) == false && ch->isOperator(clientID) == false)
        return (DataControler::SendMsg(this->clientID,ERR_NOTONCHANNEL(cl->getNickName(),ch->getChannelName())));
    if (ch->isTopicSet() == true && ch->isOperator(clientID) == false)
        return (DataControler::SendMsg(this->clientID,ERR_CHANOPRIVSNEEDED(cl->getNickName(),ch->getChannelName())));
    if (parts.size() == 1)
        return (DataControler::SendMsg(this->clientID,RPL_TOPIC(cl->getNickName(),ch->getChannelName(),ch->getTopic())));
    if (parts[1].size() > TOPICLEN)
        parts[1] = parts[1].substr(0,TOPICLEN);
    if (ch->isOperator(clientID) == false)
        return (DataControler::SendMsg(this->clientID,ERR_CHANOPRIVSNEEDED(cl->getNickName(),ch->getChannelName())));
    if (parts[1][0] == ':')
        parts[1].erase(0,1);
    ch->setTopic(clientID,parts[1]);
    DataControler::SendMsg(ch->getChannelName(),RPL_TOPIC(cl->getNickName(),ch->getChannelName(),ch->getTopic()));
}

//-----------------------------------------------------------

PrivmsgCommand::PrivmsgCommand() : Command(0) {};
PrivmsgCommand::~PrivmsgCommand() {};
PrivmsgCommand::PrivmsgCommand(int clientID, const std::string& message) : Command(clientID) {
    this->command = PRIVMSG;
    this->message = message;
};

void PrivmsgCommand::execute() {
    std::vector<std::string> parts;
    Clients *cl = DataControler::getClient(clientID);
    if (this->message.empty() == true)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"PRIVMSG")));
    parts = splitParts(this->message, 2);
    if (parts.size() < 2)
        return (DataControler::SendMsg(this->clientID,ERR_NEEDMOREPARAMS(cl->getNickName(),"PRIVMSG")));
    this->target = split(parts[0], ',');
    for (size_t i = 0; i < this->target.size(); i++) {
        if (this->target[i][0] == '#'){
            if (DataControler::channelnamesExist(this->target[i].substr(1,this->target[i].length())) == false){
                    DataControler::SendMsg(this->clientID,ERR_NOSUCHCHANNEL(cl->getNickName(),this->target[i]));
                    continue;
            }
            Channels *ch = DataControler::getChannel(this->target[i].erase(0,1));
            if (ch->isMember(clientID) == false && ch->isOperator(clientID) == false){
                DataControler::SendMsg(this->clientID,ERR_CANNOTSENDTOCHAN(cl->getNickName(),ch->getChannelName()));
                continue;
            }
            DataControler::SendMsg(ch->getChannelName(),this->clientID,RPL_PRIVMSG(user_id(cl->getNickName(),cl->getUserName()), "#" + ch->getChannelName(),parts[1]));
        }
        else{
            if (DataControler::isClient(this->target[i]) == false){
                DataControler::SendMsg(this->clientID,ERR_NOSUCHNICK(cl->getNickName(),this->target[i]));
                continue;
            }
            Clients *_cl = DataControler::getClient(this->target[i]);
            DataControler::SendMsg(_cl->getID(),RPL_PRIVMSG(user_id(cl->getNickName(),cl->getUserName()),_cl->getNickName(),parts[1]));
        }
    }
}
//-----------------------------------------------------------