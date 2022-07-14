#include <iostream>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>


int main()
{
	/*
		1. Create a socket
		2. Bin the socket to an IP port
		3. mark the socket for listenning in
		4. Accept a call
		5. close sthe listenning socket
		6. While receiving display message echo message
		7. close socket
	*/
	 //1 . Create a socket
	int listenning = socket(AF_INET, SOCK_STREAM, 0);
	if (listenning < 0)
	{
		std::cerr << "Error calling socket function" << std::endl;
		return (-1);
	}

	//2. Bind the socket to an IP port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);// stands for host to network short
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); // internet command ->  pointer to string to a number (number to an arry of integer), the 0.0.0.0 links 
	if (bind(listenning, (sockaddr*)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Can't bid to IP/port" << std::endl;
		return (-1);
	}

	// 3 . check if the socket is listenning in
	if (listen(listenning, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen" << std::endl;
		return (-1);
	}

	//4. accept a call
	sockaddr_in client;
	socklen_t clientsize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];

	// our new client socket
	int clientSocket = accept(listenning, (sockaddr*)&client, &clientsize);	

	if (clientSocket == -1)
	{
        std::cerr << "Problem with client connecting" << std::endl;
		return (-4);
	}
	close(listenning);

	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXSERV);

	int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

	if (result)
	{
		std::cout << host << " connected on " << svc << std::endl;
		return (-1);
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	}

	char buffer[4096];
	int byteRecv;
	while (true)
	{
		// clear the buffer
		memset (buffer, 0, 4096);
		// wait for a message
		byteRecv = recv(clientSocket, buffer, 4096, 0);	
		if (byteRecv == -1)
		{
			std::cerr << "There was a connection issue " << std::endl;
			return (-1);
		}
		if (byteRecv == 0)
		{
			std::cout << "The client was disconnected" << std::endl;
			break;
		}
		//display message
		std::cout << "Recieved : " << std::string(buffer, 0, byteRecv) << std::endl;
	}
	return (0);
}
