#ifndef OPERATOR_HPP
#define OPERATOR_HPP
#include <string>
#include <iostream>
#include "../user.hpp"

class Operator : public User
{
public:
    Operator();
    Operator(User const & rhs, std::string);
    ~Operator();
    Operator &operator=(Operator const & rhs);

private:
	std::string _name;
};

#endif