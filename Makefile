NAME     = ircserv
CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
SRCS     = src/main.cpp \
		   src/server/Server.cpp src/server/Socket.cpp src/server/SocketIO.cpp src/server/SocketDisconnect.cpp \
		   src/client/Client.cpp \
		   src/channel/Channel.cpp \
		   src/command/CommandParser.cpp src/command/AuthCommands.cpp \
		   src/command/ChannelCommands.cpp src/command/OperatorCommands.cpp src/command/ModeCommands.cpp \
		   src/command/MessageCommands.cpp
OBJDIR   = obj
OBJS     = $(SRCS:src/%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)


$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I include -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
