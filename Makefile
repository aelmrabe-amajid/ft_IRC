SRCS = $(shell find src -name '*.cpp')
INC = inc
OBJDIR = build/obj
DEPDIR = build/dep
OBJS = $(SRCS:src/%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:src/%.cpp=$(DEPDIR)/%.d)
VRB = -Wall -Wextra -Werror -g -fsanitize=address -std=c++98
CFLAGS = $(VRB) -std=c++98 -I $(INC)
CPP = c++
NAME = ircserv

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR) $(DEPDIR)
	$(CPP) $(CFLAGS) -MMD -MP -c $< -o $@
	mv $(OBJDIR)/$*.d $(DEPDIR)/$*.d

$(OBJDIR) $(DEPDIR):
	mkdir -p $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR) $(DEPDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all