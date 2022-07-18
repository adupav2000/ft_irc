<<<<<<< HEAD
NAME	=	ft_irc
SRCS	=	main.cpp Server/Server.cpp
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
=======
run:
	clang++ -std=c++98  -Wall -Wextra -Werror srcs/main.cpp srcs/server/server.cpp -o server

main:
	clang++ -std=c++98  -Wall -Wextra -Werror srcs/main.cpp -o server

test:
	clang++ -std=c++98 -Wall -Wextra -Werror srcs/client_test.cpp -o client
>>>>>>> 7060e80cfb47053a93339f9bb8c1d3094ac4d245
