SRCS=  src/main.cpp src/Server.cpp src/irccontrole/CChannel.cpp src/irccontrole/CClient.cpp src/irccontrole/CServer.cpp src/irccontrole/IRCControle.cpp src/irccontrole/IRCMessage.cpp src/irccontrole/IRCResponse.cpp src/irccontrole/IRCUser.cpp src/irccontrole/IRCServer.cpp src/irccontrole/IRCChannel
SRCS= src/main.cpp src/Server.cpp src/irccontrole/CChannel.cpp src/irccontrole/CClient.cpp src/irccontrole/irccontrole.cpp
INC= inc
OBJS= $(SRCS:.cpp=.o)
CFLAGS= -Wall -Wextra -Werror -std=c++98 
CPP= c++
NAME= ircserv

.PHONY: clean

all: $(NAME)

%.o: %.cpp src/Server.hpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CPP) $(CFLAGS) -I $(INC) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all