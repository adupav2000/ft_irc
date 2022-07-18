#include <iostream>
#include <errno.h>
#include <string.h>

int main ()
{
    std::cout << "error test" << strerror(401) << std::endl;
    return (0);
}
