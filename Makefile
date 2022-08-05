NAME = ircserv

MAIN_FILE = main.cpp

CLIENT_FILE = client.cpp\
			channelOperations.cpp\
			connectionRegistration.cpp\
			miscellaneousMessages.cpp\
			sendingMessages.cpp\
			serverQueriesAndCommands.cpp\
			serviceQueriesAndCommands.cpp\
			userBasedQueries.cpp \

UTILS_FILE = utils.cpp

SERVER_FILE = server.cpp

COMMAND_FILE = command.cpp

CHANNEL_FILE = channel.cpp

CLIENT_INC = client.hpp\

SERVER_INC = server.hpp

COMMAND_INC = command.hpp

GLOBAL_INC = global.hpp

CHANNEL_INC = channel.hpp

INCLUDE = $(addprefix srcs/client/, $(CLIENT_INC))
INCLUDE += $(addprefix srcs/server/, $(SERVER_INC))
INCLUDE += $(addprefix srcs/command/, $(COMMAND_INC))
INCLUDE += $(addprefix srcs/channel/, $(CHANNEL_INC))
INCLUDE += $(addprefix srcs/, $(GLOBAL_INC))


SOURCE = $(addprefix srcs/client/, $(CLIENT_FILE))
SOURCE += $(addprefix srcs/server/, $(SERVER_FILE))
SOURCE += $(addprefix srcs/command/, $(COMMAND_FILE))
SOURCE += $(addprefix srcs/channel/, $(CHANNEL_FILE))
SOURCE += $(addprefix srcs/, $(UTILS_FILE))
SOURCE += $(addprefix srcs/, $(MAIN_FILE))

COMPILER = clang++

FLAGS = -std=c++98 -Wall -Wextra -Werror -g

OBJ = $(SOURCE:.cpp=.o)

%.o:	%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@$(COMPILER) $(FLAGS)  $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
