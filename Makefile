SRCS=  src/main.cpp src/Server.cpp
OBJS= $(SRCS:.cpp=.o)
CFLAGS= -Wall -Wextra -Werror -std=c++98 
CPP= c++
NAME= ircserv

.PHONY: clean

all: $(NAME)

%.o: %.cpp src/Server.hpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CPP) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all