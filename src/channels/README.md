

## Server User :

All the Server User, are identified using ID (file descriptor). The id it's slef will be used as key inside a map 
The value will be Client Class Object.

1. Client Class Object:

```hpp
class Client //-> class for client
{
	private:
	    int id;
	    std::string IPadd;
	    std::string data;
	    std::string message;
		bool state;
		std::string userName;
		std::string nickName;
		std::string realName;
	public:
	    Client(){};
		~Client();
		Client&operator=(const Client& obj);

	    int GetFd(){return id;}
	    void SetFd(int fd){id = fd;}
	    void setIpAdd(std::string ipadd){IPadd = ipadd;}
	    void setMessage(std::string msg){message = msg;}
	    std::string getMessage(){return message;}
				
		//----------getters-----------
		bool getstate() const;
		std::string getuserName() const;
		std::string getnickName() const;
		std::string getrealName() const;
		int getuserId(void) const;

		//----------setters-----------
		void setState(bool _state);
		void setuserName(const std::string &userName);
		void setnickName(const std::string &nickName);
		void setrealName(const std::string &realName);
		void setuserId(int fd);
};
```

NOTE: The First Stage after Lanuching The Server, is Get Connection from Client. The Have just two possible cases:
	1. Client will Connect to Server without providing any User Information. (nc) 
	2. Client will Connect to Server with User Information. (Limechat)
So at the 1st case, if Client Send any Data(Command,Content) the Server will Ask for User Information. (NickName). 
At the Both Cases if The Client Want to Set User Information(UserName, RealName) There is USER command.


