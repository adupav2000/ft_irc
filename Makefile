NAME	=	ft_irc
SRCS	=	main.cpp \
			./srcs/server/server.cpp \
			./srcs/client/client.cpp \
			./srcs/command/command.cpp \
			./srcs/utils.cpp

CC		=	c++
CFLAGS	=	-Wall -Wextra -Werror
OBJS	=	$(SRCS:.cpp=.o)

%.o:		%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
