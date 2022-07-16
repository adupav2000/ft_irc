#include "Server/Server.hpp"

int main ()
{
    Server serv = Server();
	serv.init();
	serv.launch();
	return 0;
}