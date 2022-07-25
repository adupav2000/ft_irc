NAME = ft_irc
CLIENT_FILE = client.cpp\
			connectionRegistration.cpp\
			service/service.cpp\
			user/user.cpp\
			user/operator/operator.cpp \

UTILS_FILE = utils.cpp

SERVER_FILE = server.cpp

COMMAND_FILE = command.cpp

CHANNEL_FILE = channelOperations.cpp\
				channel.cpp

CLIENT_INC = client.hpp\
			  service/service.hpp\
			  user/user.hpp\
			  user/operator/operator.hpp \

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

OBJ = $(addprefix obj/, $(notdir $(SOURCE:.cpp=.o)))

FLAGS = -std=c++98 -Wall -Wextra -Werror -g 
COMPILER = clang++

# Colors
_GREY=$'\x1b[30m'
_RED=$'\x1b[31m'
_GREEN=$'\x1b[32m'
_YELLOW=$'\x1b[33m'
_BLUE=$'\x1b[34m'
_PURPLE=$'\x1b[35m'
_CYAN=$'\x1b[36m'
_WHITE=$'\x1b[37m'

all: $(NAME)

$(OBJ):
	@mkdir -p obj/
	@$(COMPILER) $(FLAGS) -c $(SOURCE)
	@mv *.o ./obj/
	@echo "${_GREEN}Creating obj files${_WHITE}"
	@echo "${_GREEN}Moving obj files in right place${_WHITE}"

$(NAME): $(OBJ)
	@$(COMPILER) $(FLAGS) srcs/main.cpp $(OBJ) -o $(NAME)
	@echo "${_GREEN}Compiling${_WHITE}"
	@echo "${_GREEN}${NAME} executable was created${_WHITE}"

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all $(NAME) main test clean fclean re
