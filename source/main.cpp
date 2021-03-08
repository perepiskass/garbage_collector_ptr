#include <iostream>
#include "gcp.h"

int main()
{
    GCP<int> p;

    try
    {
        p = new int(33);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "Value at p is: " << *p << std::endl;

    return 0;
}