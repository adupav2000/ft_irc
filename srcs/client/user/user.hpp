/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 15:31:47 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/20 15:31:48 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef User_HPP
#define User_HPP
#include <string>
#include <iostream>
#include "../client.hpp"

class User : public Client
{
public:
    ~User();
	int handle_command();

private:

// should first give the PASS
// then NICK / User ...

};

#endif