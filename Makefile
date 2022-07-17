run:
	clang++ -std=c++98  -Wall -Wextra -Werror srcs/main.cpp srcs/server/server.cpp -o server

main:
	clang++ -std=c++98  -Wall -Wextra -Werror srcs/main.cpp -o server

test:
	clang++ -std=c++98 -Wall -Wextra -Werror srcs/client_test.cpp -o client
