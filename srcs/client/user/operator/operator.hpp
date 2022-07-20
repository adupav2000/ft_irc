#ifndef OPERATOR_HPP
#define OPERATOR_HPP
#include <string>
#include <iostream>
#include "../user.hpp"

class Operator : public User
{
public:
    Operator();
    Operator(Operator const & rhs);
    ~Operator();
    Operator &operator=(Operator const & rhs);

private:

};

#endif