#include <iostream>
#include "gcp.h"
#include <vector>

int main()
{
    try
    {
        std::vector<GCP<int>> vec;
        vec.reserve(3);
        std::cout << "Capacity vector " << vec.capacity() << std::endl;
        vec.emplace_back(new int(1));
        vec.emplace_back(new int(2));
        vec.emplace_back(new int(3));

        for(auto& i : vec)
            std::cout << *i << std::endl;

        std::cout << "+++++++++++++++++++++++++++++++++++++++" << std::endl;
        vec.emplace_back(new int(4));
        std::cout << "Capacity vector " << vec.capacity() << std::endl;
        for(auto& i : vec)
            std::cout << *i << std::endl;
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    




    return 0;
}