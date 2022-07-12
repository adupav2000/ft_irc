/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 11:02:06 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/12 11:11:29 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include <string>
#include <iostream>
class server
{
public:
    server();
    server(server const & rhs);
    ~server();
    server &operator=(server const & rhs);

	/* Handle incomming strings */
	// in charge of calling the right function
	int handle_string(std::string input);

	/* client operation */
	int new_client(); // sends a PL_WELCOME and adds a client in map
	int new_service(); // sends a RPL_YOURESERVICE and adds a service in maps
	int register_password();

	

private:
	std::string name;
	//Client map<int, std::string> client_list;
	//Service map<int, std::string> service_list;
};

#endif