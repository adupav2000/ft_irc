#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <iostream>
class user : public map
{
public:
    user();
    user(user const & rhs);
    ~user();
    user &operator=(user const & rhs);

private:
// should first give the PASS
// then NICK / USER ...

};

#endif