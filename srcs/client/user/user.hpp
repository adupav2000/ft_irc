/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 15:31:47 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/27 14:17:57 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef User_HPP
#define User_HPP
#include <string>
#include <iostream>
#include "../client.hpp"

class Client;

class User : public Client
{
public:
    ~User();
	User(Client const &, std::string, std::string);
	int handle_command();
	std::string getUsername() const;
	std::string getRealname() const;

protected:
	bool			_userRegistered;
	std::string		_username;
	std::string		_realname;
	
	int OPER(Command arguments);
// should first give the PASS
// then NICK / User ...

};

#endif