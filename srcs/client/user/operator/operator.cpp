/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adu-pavi <adu-pavi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 15:51:20 by adu-pavi          #+#    #+#             */
/*   Updated: 2022/07/27 14:18:07 by adu-pavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "operator.hpp"

Operator::~Operator()
{

}

Operator::Operator(User const & rhs, std::string name) : User(rhs, rhs.getUsername(), rhs.getRealname()), _name(name)
{
	
}
