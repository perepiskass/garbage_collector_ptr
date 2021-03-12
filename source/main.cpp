#include <iostream>
#include "gcp.h"

int main()
{
    {
    GCP<int> a(new int(77));
    GCP<int> d(new int(55));
    GCP<int> pp;
    GCP<int> p(pp);

    try
    {
        p = new int(33);
        p = std::move(a);
        int* ptr_i = new int(99);
        p = ptr_i;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "Value at p is: " << *p << std::endl;
    }


    return 0;
}