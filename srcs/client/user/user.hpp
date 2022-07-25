/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 15:31:47 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/23 16:18:21 by adu-pavi         ###   ########.fr       */
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
	User(Client const & rhs);
	int handle_command();

protected:
	bool			_userRegistered;
	std::string		_username;
	

// should first give the PASS
// then NICK / User ...

};

#endif